#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <sstream>

#define SHORT_LOAD    'l'
#define SHORT_ADD     'a'
#define SHORT_MEMBER  'm'
#define SHORT_MAXIMUM 'x'
#define SHORT_QUIT   'q'

using namespace std;

static unsigned int myRank;

HashMap local;// Creo un HashMap local
void nodo(unsigned int rank_param) {
    printf("Soy un nodo. Mi rank es %d \n", rank_param);
	myRank = rank_param;
	char operation;
	MPI_Status status;
	int sizeCmd;

    while (true) {
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_CHAR, &sizeCmd);
		char cmd[sizeCmd];
		MPI_Recv(&cmd, sizeCmd, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		operation = cmd[0];
		char data[sizeCmd - 1];
		if(sizeCmd > 1)
		{
			for(int i = 1; i < sizeCmd; i++)
			{
				data[i-1] = cmd[i];
			}
		}
		switch(operation){
			case SHORT_LOAD:
				nodoLoad(data);
				break;
			case SHORT_ADD:
				nodoAdd(data);
				break;
			case SHORT_MEMBER:
				nodoMember(data);
				break;
			case SHORT_MAXIMUM:
				nodoMaximum();
				break;
			case SHORT_QUIT:
				nodoQuit();
				return;
			default:
				continue;
		}
    }
}

void nodoLoad(char *data){
    ifstream myfile(data, ifstream::in);
    if (myfile.is_open())
	{
	    string word;
	    while (myfile >> word )
		{
			local.addAndInc(word);
	    }
	}
	myfile.close();
	MPI_Ssend("r", 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
}
void nodoAdd(char *data){

	int yo = myRank;
	MPI_Request req;
	trabajarArduamente();
	MPI_Isend(&yo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);

	unsigned int winnerRank;
	MPI_Status status;
	MPI_Recv(&winnerRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	if(winnerRank != myRank){
		return;
	}

	local.addAndInc(data);
}

void nodoMember(char *data){
	bool esta = local.member(data); 
	

	trabajarArduamente();


    MPI_Gather(
        &esta,
        1,
        MPI_CHAR,
        NULL,
        0,
        MPI_CHAR,
        0,
        MPI_COMM_WORLD);
}

void nodoMaximum(){
	string actual;
	int count = 0;
	char *packedCount = (char *) &count; 
	trabajarArduamente();
	for(HashMap::iterator it = local.begin(); it !=local.end(); it++){
		if(count == 0) {
			// Solo la primera vez
			actual = *it;
		}
		if(actual.compare(*it) == 0){
			count++;
		}
		else{
			//Empaquetamos la cantidad de repeticiones de la palabra a enviar.		
			actual.append(packedCount, 4);

			
			// Se estaba mandando el puntero a un string...eso puede ser cualquiera.
			const char *cactual = actual.c_str();
			MPI_Send(cactual, actual.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			actual = *it;			
			count = 1;
		}
	}

	//Un cero en char y un cero en int.
	char buffer[] = {0, 0, 0, 0, 0};
	// Hay que hacer esto...Isend no es buffereado! O ver de usar Bsend (Hay que crear de antemano un buffer...)
	MPI_Send(buffer, 5, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
}

void nodoQuit(){
	//MPI_Finalize();
}
void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}
