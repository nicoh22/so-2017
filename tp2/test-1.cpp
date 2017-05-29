#include <iostream>
#include <cstdlib>
#include "ListaAtomica.hpp"
#include <list>
#include <fstream>
using namespace std;

typedef struct threadArgs_t{
	threadArgs_t()
	{
		lista = nullptr;
	}
	Lista<string> *lista;
	pthread_mutex_t * file_locks;
	list<string> * file_names;
} threadArgs;

void  * processFile(void *args)
{
	threadArgs *arg = (threadArgs *) args;
	for (int l = 0; l < arg->file_names->size(); l++)
	{
		if(pthread_mutex_trylock(&(arg->file_locks[l])) == 0)
		{

			list<string>::iterator it = arg->file_names->begin();
			int lfin = 0;
			while(lfin<l){it++;lfin++;}
			
			string filename = *it;

			ifstream myfile(filename);
			if (myfile.is_open())
			{
				string word;
				while (myfile >> word)
				{
					arg->lista->push_front(word);
				}
			}
			myfile.close();
		}
	}

}
int main(int argc, char **argv) {
	Lista<string> h;
	list<string> l = { "corpus-0", "corpus-1", "corpus-2", "corpus-3", "corpus-4" };
	int i;

	if (argc != 2) {
		cerr << "uso: " << argv[0] << " #threads" << endl;
		return 1;
	}

	int nt = atoi(argv[1]);
	pthread_t threads[nt];
	pthread_mutex_t file_lock_list[l.size()];
	int tid;
	threadArgs args;

	//Inicializo los locks de files
	for(int i = 0; i < l.size(); i++)
	{
		pthread_mutex_init(&file_lock_list[i], NULL);
	}

	args.lista = &h;
	args.file_names = &l;
	args.file_locks = file_lock_list;
	//Asigno trabajo a los threads
	for(tid = 0; tid < nt; tid++)
	{
		pthread_create(&threads[tid], NULL, processFile, (void *)&args);	
	}

	for(int i = 0; i < nt; i++)
	{
		pthread_join(threads[i], NULL);
	}

	for(int i = 0; i < l.size(); i++)
	{
		pthread_mutex_destroy(&file_lock_list[i]);
	}

	for (auto it = h.CrearIt(); it.HaySiguiente(); it.Avanzar())
	{
		auto t = it.Siguiente();
		cout << t << endl;
	}
	return 0;
}

