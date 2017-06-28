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

using namespace std;

#define CMD_LOAD    "load"
#define CMD_ADD     "addAndInc"
#define CMD_MEMBER  "member"
#define CMD_MAXIMUM "maximum"
#define CMD_QUIT    "quit"
#define CMD_SQUIT   "q"

#define SHORT_LOAD    'l'
#define SHORT_ADD     'a'
#define SHORT_MEMBER  'm'
#define SHORT_MAXIMUM 'x'
#define SHORT_QUIT   'q'
#define FILE_READY   "r"

static unsigned int np;

//Desempaqueta un mensaje. El mismo contiene una palabra y
//su cantidad de repeticiones.
pair<string, int> parseMessage(string message){
	string word = message.substr(0, message.size() - 4);
	int count = *((int *) message.substr(message.size() - 4, 4).c_str());
	return make_pair(word, count);
}

//Recibe de forma bloqueante cualquier mensaje de cualquier nodo.
//Retorna el mensaje y el proceso que lo envio

// CORRER CON: 3 nodos (2 seran slave) y hacer load de a1.txt a2.txt a3.txt
// NOTA 1: En load siempre queda un remanente de mensajes no leidos. Hago receiveFromAnyBloq de los mismos.
// Esto tiene que hacerse además de ser necesario para liberar el buffer, porque los sends que se hacen en load por parte de los nodos son bloqueantes. Si nadie los lee quedan bloqueados.

unsigned int calculateType(MPI_Datatype datatype) {
	unsigned int mult = 1;

	switch(datatype){
		case MPI_INT:
			mult = 4;
			break;
		default:
			break;
	}

	return mult;
}

pair< string, int> receiveFromAnyBloq(MPI_Datatype datatype){
	MPI_Status status;
	MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
	int messageSize;
	MPI_Get_count(&status, datatype, &messageSize);

	unsigned int totalSize = messageSize*calculateType(datatype);	

	char *data = new char[totalSize];

	for (unsigned int i = 0; i < totalSize; i++) {
		data[i] = 0;
	}

	MPI_Recv(data, totalSize, datatype, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);

	// Descarta basuras poniendo el total que quiero leer del buffer...no se porque a veces llega basura.
	string res(data, totalSize);

	delete data;
	return make_pair(res, status.MPI_SOURCE);
}
//Empaqueta la operacion y la data a enviar.
void buildMessage(char op, const char *from, int sizeArg, char* to){
    //char to[sizeArg + 1];
    to[0] = op;
    
    for(int k = 0; k < sizeArg; k++){
        to[k + 1] = from[k];
    }

}
//Envia a todos los nodos la instruccion con el argumento data.
static void sendInst(char instruction, const char *data, int size){
    MPI_Request req[np];
	char buffer[size+1];
	buildMessage(instruction, data, size, buffer);
    for(int j = 1; ((unsigned int) j) < np; j++){
        MPI_Isend(&buffer, size + 1, MPI_CHAR, j, 0, MPI_COMM_WORLD, &req[j]);
    }
}

// Crea un ConcurrentHashMap distribuido
static void load(list<string> params) {
	//char op = SHORT_LOAD;
    list<string>::iterator it=params.begin();
    MPI_Request requests[np];

	int msgNotRead = 0;

    for(int p = 1; ((unsigned int) p) < np; p++){
        if(((unsigned int)p) > params.size()) break;
		int sizeFilename = (*it).size() + 1;
		char fileMessage[sizeFilename + 1];
		buildMessage(SHORT_LOAD, (*it).c_str(), sizeFilename, fileMessage);
		MPI_Isend(fileMessage, sizeFilename + 1, MPI_CHAR, p, 0, MPI_COMM_WORLD, &requests[p]);
        it++;
		msgNotRead++;
    }
    int p;
    while(it != params.end()){
		pair<string, int> response = receiveFromAnyBloq(MPI_CHAR);
		msgNotRead--;
		p = response.second;
		string fileReady = "r";
		if(response.first.compare(fileReady) == 0 ){
			int sizeFilename = (*it).size();
			char fileMessage[sizeFilename + 1];
			buildMessage(SHORT_LOAD, (*it).c_str(), sizeFilename, fileMessage);
			MPI_Isend(fileMessage, sizeFilename + 1, MPI_CHAR, p, 0, MPI_COMM_WORLD, &requests[p]);
			it++;
			msgNotRead++;
		}
	}


	for(int i = 0; i < msgNotRead; i++){
		receiveFromAnyBloq(MPI_CHAR);//Liberamos buffer de mensajes
	}

    cout << "La lista esta procesada" << endl;
}

// Esta función debe avisar a todos los nodos que deben terminar
static void quit() {
	char op = SHORT_QUIT;
	//MPI_Bcast(&op, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
	sendInst(op, NULL, 0);
}

// Esta función calcula el máximo con todos los nodos
static void maximum() {

	HashMap hmMax;
    pair<string, unsigned int> result;
    
//	bool nReadyArr[np - 1];
	unsigned int nReadyCount = 0;

	char op = SHORT_MAXIMUM;
	sendInst(op, NULL, 0);

	while(nReadyCount < np-1){
		pair<string, int> response = receiveFromAnyBloq(MPI_CHAR);
		pair<string, int> message = parseMessage(response.first);
		int process = response.second;
		string word = message.first;
		int count = message.second;


		if(count == 0){
//			nReadyArr[process - 1] = true;
			nReadyCount++;

			continue;
		}

			
		for(int i = 0; i < count; i++){			
			hmMax.addAndInc(word);
		}
		
	}

	result = hmMax.maximum();

    cout << "El máximo es <" << result.first <<"," << result.second << ">" << endl;
}

// Esta función busca la existencia de *key* en algún nodo
static void member(string key) {
    bool esta = false;
    bool nodos[np];
	int size = key.size();
	char op = SHORT_MEMBER;
	sendInst(op, key.c_str(), size);

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

    //int p = chooseProcess();
    //char buffer[key.size()];

	/*
    buffer[0] = SHORT_ADD;
    for(unsigned int k = 0; k < key.size(); k++){
        buffer[k + 1] = key[k];
    }
	*/

	sendInst(SHORT_ADD, key.c_str(), key.size() + 1);

	pair<string, int> winner = receiveFromAnyBloq(MPI_INT); 
	int winnerProcess = winner.second; 
    MPI_Request req[np];

    for(unsigned int j = 1; j < np; j++){
        MPI_Isend(&winnerProcess, 1, MPI_INT, j, 0, MPI_COMM_WORLD, &req[j]);
    }

	for(unsigned int i = 0; i < np-2; i++){
		//TODO: si tengo un solo nodo funca todo bien?
		//RTA: si tenes un solo nodo ni entra el for: i < 2-2 = 0
		receiveFromAnyBloq(MPI_INT);//Liberamos buffer de mensajes
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
