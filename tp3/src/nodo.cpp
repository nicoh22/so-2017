#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>

#define SHORT_LOAD    'l'
#define SHORT_ADD     'a'
#define SHORT_MEMBER  'm'
#define SHORT_MAXIMUM 'x'
#define SHORT_QUIT   'q'
using namespace std;

HashMap local;// Creo un HashMap local
void nodo(unsigned int rank) {
    printf("Soy un nodo. Mi rank es %d \n", rank);

	char operation;
	MPI_Status status;
	int sizeCmd;

    while (true) {
		//MPI_Bcast(&operation, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
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
				nodoLoad(&data);
				break;
			case SHORT_ADD:
				nodoAdd(&data);
				break;
			case SHORT_MEMBER:
				nodoMember(&data);
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
    std::ifstream myfile(data);
    if (myfile.is_open())
	{
		std::string word;
	    while (myfile >> word )
		{
			local.addAndInc(word);
	    }
	}
	myfile.close();
	MPI_Send("r", 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
}
void nodoAdd(char *data){

	int yo = world_rank;
	MPI_REQUEST req;
	trabajarArduamente();
	MPI_Isend(&yo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);

	int winnerRank;
	MPI_Recv(&winnerRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	if(winnerRank != world_rank){
		return;
	}

	local.addAndInc(data);
}

void nodoMember(){
	int size;
	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	char key[size];
	MPI_Bcast(&key, size, MPI_CHAR, 0, MPI_COMM_WORLD);

	bool esta = local.member(key); 
	bool nodos[world_size];

	trabajarArduamente();
    MPI_Gather(
        &esta,
        1,
        MPI_CHAR,
        &nodos,
        world_size,
        MPI_CHAR,
        0,
        MPI_COMM_WORLD);
}

void nodoMaximum(){
	HashMap::iterator it;
	MPI_REQUEST req;
	string actual;
	int count = 0;
	char *packedCount = (char *) &count; 
	trabajarArduamente();
	for(it = local.begin(); it !=local.end(); it++){
		if(count == 0){
			actual = *it;
		}
		if(strncmp(actual, *it) == 0){
			count++;
		}
		else{
			//Empaquetamos la cantidad de repeticiones de la palabra a enviar.
			actual.append(packedCount, 4);
			MPI_Isend(&actual, actual.size()+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &req);
			count = 0;
		}
	}
	char buffer = {0, 0};
	MPI_Isend(&buffer, 2, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &req);
}

void nodoQuit(){
	MPI_Finalize();
}
void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}
