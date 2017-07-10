#ifndef _CONSTANTS_H
#define _CONSTANTS_H
#include <string.h>
using namespace std;

#define CONSOLA 0

enum Tag { LOAD = 1, ADD = 2, MEMBER = 3, MAXIMUM = 4, QUIT = 5, READY = 6, ID = 7, WORDMESSAGE = 8 };

typedef unsigned int Rank;

struct Message {
	Tag tag;
	Rank rank;
	string message;
	string word;
	int wordCount;

	Message(MPI_Status status, char* msj, int size){
		message = string(msj, size);
		tag = (Tag)status.MPI_TAG;
		rank = status.MPI_SOURCE;
		if(tag==WORDMESSAGE){
			word = message.substr(0, message.size() - 4);
			wordCount = *((int *) message.substr(message.size() - 4, 4).c_str());
		}


	}
};


#endif 