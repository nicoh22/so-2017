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

#define SHORT_LOAD    "l"
#define SHORT_ADD     "a"
#define SHORT_MEMBER  "m"
#define SHORT_MAXIMUM "x"
#define SHORT_QUIT   "q"

static unsigned int np;

static void sendInst(char &instruction, char *data, int size){
    char buffer[size + 1];
    buffer[0] = instruction;
    
    for(int k = 0; k < size; k++){
        buffer[k + 1] = data[k];
    }

    MPI_REQUEST req[world_size];
    for(int j = 1; j < world_size; j++){
        MPI_Isend(&buffer, size + 1, MPI_CHAR, j, 0, MPI_COMM_WORLD, &req[j]);
    }
}

//for (list<string>::iterator it=params.begin(); it != params.end(); ++it) {
// Crea un ConcurrentHashMap distribuido
static void load(list<string> params) {
    FILE archivos[world_size];
    int i = 0;
    list<string>::iterator it=params.begin();
    for(int p = 0; p < world_size; p++){
        if(p>=params.size()) break;
        archivos[p] = fopen(*it.c_str(), 'r');
        it++;
    }
    string read_word;
    int processed = 0;
    int p = 1;
    it=params.begin();
    int ready;//flag
    MPI_REQUEST requests[world_size];
    string buffer;
    while(processed < params.size()){
        string actual;
        actual << archivos[p];
        if(strncmp(actual, EOF) == 0){ //TODO: eof
            processed++;
            if(it == params.end()){
                archivos[p] = fopen(*it.c_str(), 'r');
            }
        }
        else{
            actual.push_back(SHORT_ADD);
            //falta el buffer para la palabra, preguntar si termino el send anterior. declarar los request.
            MPI_Test(&requests[p], &ready, MPI_STATUS_IGNORE);
            if(ready){//
                MPI_Issend(actual.c_str(), actual.size(), MPI_CHAR, p, 0, MPI_COMM_WORLD, &requests[p]);
            }
        }
        p = ((p + 1) % world_size - 1) + 1;
    }

    
/*
    for (list<string>::iterator it=params.begin(); it != params.end(); ++it) {
       
    }
*/
    cout << "La listá esta procesada" << endl;
}

// Esta función debe avisar a todos los nodos que deben terminar
static void quit() {
	char op = SHORT_QUIT;
	MPI_Bcast(&op, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Finalize();
}

// Esta función calcula el máximo con todos los nodos
static void maximum() {

	HashMap hmMax;
    pair<string, unsigned int> result;
    
	bool nReadyArr[world_size - 1];
	int nReadyCount = 0;

	char op = SHORT_MAXIMUM;
	MPI_REQUEST req;
	MPI_Ibcast(&op, 1, MPI_CHAR, 0, MPI_COMM_WORLD, req);

	while(nReadyCount < world_size){
		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		int messageSize;
		MPI_Get_count(&status, MPI_INT, &messageSize);

		char data[messageSize];//Puede cambiar

		MPI_Recv(&data, messageSize, MPI_CHAR, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
		
		char word = strncpy(data-4);//pseudo
		int count = (int) *(data-4);

		if(count == 0){
			nReadyArr[status.MPI_TAG - 1] = true;
			nReadyCount++;
			continue;
		}

		for(int i = 0; i < count; i++){
			hmMax.addAndInc(word);
		}

	}

//	sendInst(SHORT_MAXIMUM, NULL, 0);

	result = hmMax.maximum();
    //string str("a");
    //result = make_pair(str,10);

    cout << "El máximo es <" << result.first <<"," << result.second << ">" << endl;
}

// Esta función busca la existencia de *key* en algún nodo
static void member(string key) {
    bool esta = false;
    bool nodos[world_size];
    sendInst(SHORT_MEMBER, NULL, 0);

	int size = key.size();

	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&key.c_str(), size, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_Gather(
        &esta,
        1,
        MPI_CHAR,
        &nodos,
        world_size,
        MPI_CHAR,
        0,
        MPI_COMM_WORLD);

    for(int i = 0; i < world_size; i++){
        if(nodos[i]){
            esta = true;
            break;  
        } 
    }

    cout << "El string <" << key << (esta ? ">" : "> no") << " está" << endl;
}


// Esta función suma uno a *key* en algún nodo
static void addAndInc(string key) {

    int p = chooseProcess();
    char buffer[key.size()];

    buffer[0] = SHORT_ADD;
    for(int k = 0; k < size; k++){
        buffer[k + 1] = key[k];
    }
    MPI_REQUEST req[world_size];
    MPI_Isend(&buffer, key.size() + 1, MPI_CHAR, p, 0, MPI_COMM_WORLD, &req[j]);



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
