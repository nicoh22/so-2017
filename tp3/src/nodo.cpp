#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>

#define SHORT_LOAD    "l"
#define SHORT_ADD     "a"
#define SHORT_MEMBER  "m"
#define SHORT_MAXIMUM "x"
#define SHORT_QUIT   "q"
using namespace std;

HashMap local;
void nodo(unsigned int rank) {
    printf("Soy un nodo. Mi rank es %d \n", rank);

    // TODO: Implementar
    // Creo un HashMap loal

	char operation;
    while (true) {
		MPI_Bcast(&operation, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
		switch(operation){
			case SHORT_LOAD:
				nodoLoad();
				break;
			case SHORT_ADD:
				nodoAdd();
				break;
			case SHORT_MEMBER:
				nodoMember();
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

void nodoAdd(){
	//double timestamp = MPI_Wtime();
	//double times[world_size];
	//MPI_Gather(&timestamp, 1, MPI_DOUBLE, &times, world_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	int yo = world_rank;
	MPI_REQUEST req;
	MPI_Isend(&yo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);

	int winnerRank;
	MPI_Bcast(&winnerRank, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if(winnerRank != world_rank){
		return;
	}

	MPI_Status status;
	MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
	int sizeKey;
	MPI_Get_count(&status, MPI_CHAR, sizeKey);
	char key[sizeKey];

	MPI_Recv(&key, sizeKey, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	local.addAndInc(key);

	trabajarArduamente();
}
void nodoQuit(){
	MPI_Finalize();
}
void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}
