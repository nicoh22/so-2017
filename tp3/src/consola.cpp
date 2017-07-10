#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include "consola.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include "utilitarios.hpp"

using namespace std;

#define CMD_LOAD    "load"
#define CMD_ADD     "addAndInc"
#define CMD_MEMBER  "member"
#define CMD_MAXIMUM "maximum"
#define CMD_QUIT    "quit"
#define CMD_SQUIT   "q"



static unsigned int np;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

                        //UTILITARIOS//

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

Message receiveFromAnyBloq(){
	MPI_Status status;
	MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	int messageSize;
	MPI_Get_count(&status, MPI_CHAR, &messageSize);
	char data[messageSize];
	MPI_Recv(data, messageSize, MPI_CHAR, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
	
	return Message(status, data, messageSize);
}

//Envia a todos los nodos la instruccion con el argumento data.
static void sendInst(Tag tag, const string &data){
    MPI_Request req[np];
    for(int j = 1; ((unsigned int) j) < np; j++){
        MPI_Isend(data.c_str(), data.size() + 1, MPI_CHAR, j, tag, MPI_COMM_WORLD, &req[j]);
    }
}
static void sendInst(Tag tag, int data){
    MPI_Request req[np];
    for(Rank j = 1; j < np; j++){
        MPI_Isend(&data, 1 , MPI_INT, j, tag, MPI_COMM_WORLD, &req[j]);
    }
}

static void sendInst(Tag tag){
    MPI_Request req[np];
    for(int j = 1; ((unsigned int) j) < np; j++){
        MPI_Isend(NULL, 0, MPI_CHAR, j, tag, MPI_COMM_WORLD, &req[j]);
    }
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

                        //METODOS DE CONSOLA//

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------



// Crea un ConcurrentHashMap distribuido
static void load(list<string> params) {
    list<string>::iterator it=params.begin();
    MPI_Request requests[np];

	int msgNotRead = 0;

    for(Rank p = 1;  p < np; p++){
        if(p > params.size()) break;
		
		MPI_Isend((*it).c_str(), (*it).size() + 1, MPI_CHAR, p, LOAD, MPI_COMM_WORLD, &requests[p]);
        it++;
		msgNotRead++;
    }

    while(it != params.end()){
		Message response = receiveFromAnyBloq();
		msgNotRead--;
		if(response.tag == READY ){
			 MPI_Isend((*it).c_str(), (*it).size() + 1, MPI_CHAR, response.rank, LOAD, MPI_COMM_WORLD, &requests[response.rank]);
			it++;
			msgNotRead++;
		}
	}


	for(int i = 0; i < msgNotRead; i++){
		receiveFromAnyBloq();//Liberamos buffer de mensajes
	}

    cout << "La lista esta procesada" << endl;
}

// Esta función debe avisar a todos los nodos que deben terminar
static void quit() {
	sendInst(QUIT);
}

// Esta función calcula el máximo con todos los nodos
static void maximum() {
	HashMap hmMax;
    pair<string, unsigned int> result;
    
//	bool nReadyArr[np - 1];
	unsigned int nReadyCount = 0;

	sendInst(MAXIMUM);

	while(nReadyCount < np-1){
		Message response = receiveFromAnyBloq();

		if(response.tag == READY){
			nReadyCount++;
			
		}else{

    		for(int i = 0; i < response.wordCount; i++){			
    			hmMax.addAndInc(response.word);
    		}
            
        }

		
	}

	result = hmMax.maximum();

    cout << "El máximo es <" << result.first <<"," << result.second << ">" << endl;
}

// Esta función busca la existencia de *key* en algún nodo
static void member(string key) {
    bool esta = false;
    bool nodos[np];

	sendInst(MEMBER, key);

    MPI_Gather(
        &esta,
        1,
        MPI_CHAR,
        &nodos,
        1,
        MPI_CHAR,
        0,
        MPI_COMM_WORLD);

    for(unsigned int i = 0; i < np; i++){
        if(nodos[i]){
            esta = true;
            break;  
        } 
    }

    cout << "El string <" << key << (esta ? ">" : "> no") << " está" << endl;
}


// Esta función suma uno a *key* en algún nodo
static void addAndInc(string key) {
	sendInst(ADD, key);
    
	Message winnerMessage = receiveFromAnyBloq(); 
	Rank winnerProcess = winnerMessage.rank; 
    
    sendInst(ID, winnerProcess);
    
	for(Rank i = 0; i < np-2; i++){
		//TODO: si tengo un solo nodo funca todo bien?
		//RTA: si tenes un solo nodo ni entra el for: i < 2-2 = 0
		receiveFromAnyBloq();//Liberamos buffer de mensajes
	}

    cout << "Agregado: " << key << endl;
}


/* static int procesar_comandos()
La función toma comandos por consola e invoca a las funciones correspondientes
Si devuelve true, significa que el proceso consola debe terminar
Si devuelve false, significa que debe seguir recibiendo un nuevo comando
*/

static bool procesar_comandos() {

    char buffer[BUFFER_SIZE];
    size_t buffer_length;
    char *res, *first_param, *second_param;

    // Mi mamá no me deja usar gets :(
    res = fgets(buffer, sizeof(buffer), stdin);

    // Permitimos salir con EOF
    if (res==NULL)
        return true;

    buffer_length = strlen(buffer);
    // Si es un ENTER, continuamos
    if (buffer_length<=1)
        return false;

    // Sacamos último carácter
    buffer[buffer_length-1] = '\0';

    // Obtenemos el primer parámetro
    first_param = strtok(buffer, " ");

    if (strncmp(first_param, CMD_QUIT, sizeof(CMD_QUIT))==0 ||
        strncmp(first_param, CMD_SQUIT, sizeof(CMD_SQUIT))==0) {

        quit();
        return true;
    }

    if (strncmp(first_param, CMD_MAXIMUM, sizeof(CMD_MAXIMUM))==0) {
        maximum();
        return false;
    }

    // Obtenemos el segundo parámetro
    second_param = strtok(NULL, " ");
    if (strncmp(first_param, CMD_MEMBER, sizeof(CMD_MEMBER))==0) {
        if (second_param != NULL) {
            string s(second_param);
            member(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_ADD, sizeof(CMD_ADD))==0) {
        if (second_param != NULL) {
            string s(second_param);
            addAndInc(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_LOAD, sizeof(CMD_LOAD))==0) {
        list<string> params;
        while (second_param != NULL)
        {
            string s(second_param);
            params.push_back(s);
            second_param = strtok(NULL, " ");
        }

        load(params);
        return false;
    }

    printf("Comando no reconocido");
    return false;
}

void consola(unsigned int np_param) {
    np = np_param;
    printf("Comandos disponibles:\n");
    printf("  "CMD_LOAD" <arch_1> <arch_2> ... <arch_n>\n");
    printf("  "CMD_ADD" <string>\n");
    printf("  "CMD_MEMBER" <string>\n");
    printf("  "CMD_MAXIMUM"\n");
    printf("  "CMD_SQUIT"|"CMD_QUIT"\n");

    bool fin = false;
    while (!fin) {
        printf("> ");
        fflush(stdout);
        fin = procesar_comandos();
    }
}
