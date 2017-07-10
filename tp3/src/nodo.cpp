#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <sstream>
#include "utilitarios.hpp"


using namespace std;

static Rank myRank;

HashMap local;// Creo un HashMap local


void nodo(unsigned int rank_param) {
	printf("Soy un nodo. Mi rank es %d \n", rank_param);
	myRank = rank_param;
	MPI_Status status;
	int data_size;

	while (true) {
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_CHAR, &data_size);
		char data[data_size];

		MPI_Recv(&data, data_size, MPI_CHAR, CONSOLA, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		switch(status.MPI_TAG){
			case LOAD:
				nodoLoad(data);
			break;
			case ADD:
				nodoAdd(data);
			break;
			case MEMBER:
				nodoMember(data);
			break;
			case MAXIMUM:
				nodoMaximum();
			break;
			case QUIT:
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
	MPI_Ssend(NULL, 0, MPI_CHAR, CONSOLA, READY, MPI_COMM_WORLD);
}
void nodoAdd(char *data){

	MPI_Request req;
	trabajarArduamente();
	MPI_Isend(&myRank, 1, MPI_INT, CONSOLA, ID, MPI_COMM_WORLD, &req);

	Rank winnerRank;
	MPI_Status status;
	MPI_Recv(&winnerRank, 1, MPI_INT, CONSOLA, ID, MPI_COMM_WORLD, &status);
	
	if(winnerRank == myRank){
		local.addAndInc(data);
	}

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
	for(HashMap::iterator it = local.begin(); it !=local.end(); ){
		
		actual = *it;
		count = 0;

		while(it !=local.end() && actual.compare(*it) == 0){
			count++;
			it++;
		}
		
		//Empaquetamos la cantidad de repeticiones de la palabra a enviar.		
		actual.append(packedCount, 4);
		const char *cactual = actual.c_str();
		MPI_Ssend(cactual, actual.size(), MPI_CHAR, CONSOLA, WORDMESSAGE, MPI_COMM_WORLD);
			
		
	}


	MPI_Send(NULL, 0, MPI_CHAR, CONSOLA, READY, MPI_COMM_WORLD);
}

void nodoQuit(){
	//MPI_Finalize();
}
void trabajarArduamente() {
	int r = rand() % 2500000 + 500000;
	usleep(r);
}
