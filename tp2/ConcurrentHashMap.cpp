#include "ConcurrentHashMap.hpp"
#include <atomic>
#include <utility>
#include <fstream>
typedef struct max_t{

	tupla *maximums[26];
	pthread_mutex_t locks[26];
	ConcurrentHashMap *hashMap;
	max_t()
	{
		for(int i = 0; i < 26; i++)
		{
			maximums[i] = NULL;
			pthread_mutex_init(&locks[i], NULL);
		}
	}
	~max_t()
	{
		for(int i = 0; i < 26; i++)
		{
		//	maximums[i] = NULL;
			pthread_mutex_destroy(&locks[i]);
		}
	}
} maxtarg;

typedef struct file_n_map{
	ConcurrentHashMap *hashmap;
	std::string *filename;
} fileNMap;

typedef struct lock_n_file_n_map{
	lock_n_file_n_map()
	{
		hashmap = nullptr;
	}
	ConcurrentHashMap *hashmap;
	pthread_mutex_t * file_locks;
	std::list<std::string> * file_names;
} lockNFileNMap;

ConcurrentHashMap::ConcurrentHashMap()
{
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_init(&lock_list[i], NULL);
	}
}
/*
ConcurrentHashMap::ConcurrentHashMap(const ConcurrentHashMap& other)
{
	for (int i = 0; i < 26; ++i)
	{
		Lista< tupla>::Iterador it = other.map[i].CrearIt();
		while(it.HaySiguiente())
		{
			this->
		pthread_mutex_init(&lock_list[i], NULL);
	}
	
}
*/

ConcurrentHashMap::~ConcurrentHashMap()
{
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_destroy(&lock_list[i]);
	}
}

void ConcurrentHashMap::addAndInc(std::string key)
{
	unsigned int index = hash(key);
	pthread_mutex_lock(&lock_list[index]);
	Lista< tupla >::Iterador it = tabla[index].CrearIt();
	while(it.HaySiguiente())
	{
		if(key.compare(it.Siguiente().first) == 0)
		{
			it.Siguiente().second++;
			pthread_mutex_unlock(&lock_list[index]);
			return;
		}
		it.Avanzar();
	}
	tabla[index].push_front(make_pair(key, 1));
	pthread_mutex_unlock(&lock_list[index]);
}


bool ConcurrentHashMap::member(std::string key)
{
	unsigned int index = hash(key);
	Lista< tupla  >::Iterador it = tabla[index].CrearIt();
	while(it.HaySiguiente())
	{
		if(key.compare(it.Siguiente().first) == 0)
		{
			return true;
		}
		it.Avanzar();
	}
	return false;
}

void *ConcurrentHashMap::maxThread(void *args)
{
	maxtarg *arg = (maxtarg *) args;
	ConcurrentHashMap *myMap = arg->hashMap;
	myMap->findMaximums(args);
}


void ConcurrentHashMap::findMaximums(void *args)
{
	maxtarg *arg = (maxtarg *) args;
	for(int i = 0; i < 26; i++)
	{
		if(pthread_mutex_trylock(&(arg->locks[i])) == 0)
		{
			Lista< tupla >::Iterador it = tabla[i].CrearIt();
			//esta es la unica forma de saber si ListaAtomica esta vacia:
			//Se calcula solo si no fue calculado antes (maximus[i]==NULL) y nadie lo esta analizando (lock)
			if(arg->maximums[i] == NULL && it.HaySiguiente())
			{
				tupla max = std::make_pair("", 0);
				int c = 0;
				while(it.HaySiguiente())
				{
					
					tupla *actual = &it.Siguiente();
					if (actual->second > max.second) {
						max = *actual;
					}
					it.Avanzar();
				}
				//printf("Hola %d\n", max.first );

				arg->maximums[i] = new tupla;
				arg->maximums[i]->first = max.first;
				arg->maximums[i]->second = max.second;
			}

			pthread_mutex_unlock(&arg->locks[i]);
		}
	}
}

tupla ConcurrentHashMap::maximum(unsigned int nt)
{
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_lock(&lock_list[i]);
	}
	pthread_t threads[nt];
	int tid;
	maxtarg args;
	args.hashMap = this;//Le paso la referencia de mi mismo
	for(tid = 0; tid < nt; tid++)
	{
		pthread_create(&threads[tid], NULL, &ConcurrentHashMap::maxThread, (void *)&args);	
	}
	for(int i = 0; i < nt; i++)
	{
		pthread_join(threads[i], NULL);
	}
	tupla max = tupla("", 0);
	for(int i = 0; i < 26; i++)
	{
		if(args.maximums[i] != NULL && args.maximums[i]->second > max.second)
		{
			max = *args.maximums[i];
		}
	}
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_unlock(&lock_list[i]);
		delete args.maximums[i];
	}
	//TODO: destruir locks de maxtargs, funcion del struct.
	return max;
}


unsigned int ConcurrentHashMap::hash(std::string key)
{
	return key[0] - 97;
}

ConcurrentHashMap ConcurrentHashMap::count_words(std::string archivo){
	//Hay que devolver un ConcurrentHashMap, no un puntero
	//O sea que hay que implementar el constructor por copia 
	//declarar en stack el hash map y devolverlo
	//pthread_mutex_lock(&count_words_lock);
    std::ifstream myfile(archivo);
    ConcurrentHashMap hashmap;
    if (myfile.is_open())
	{
		std::string word;
	    while (myfile >> word )
		{
			hashmap.addAndInc(word);
	    }
	}
	myfile.close();
	//pthread_mutex_unlock(&count_words_lock);
	return hashmap;

};

void * ConcurrentHashMap::count_words_Thread(void *args)
{
	fileNMap *arg = (fileNMap *) args;
	std::ifstream myfile(*arg->filename);
	if (myfile.is_open())
	{
		std::string word;
	    while (myfile >> word )
		{
			arg->hashmap->addAndInc(word);
	    }
	}
	myfile.close();

}

ConcurrentHashMap ConcurrentHashMap::count_words(std::list<std::string> archivos){
	ConcurrentHashMap  hashmap;
	std::list<std::string>::iterator it=archivos.begin();
	pthread_t threads[archivos.size()];
	int tid;
	fileNMap args[archivos.size()];
	int nt = archivos.size();

	for(tid = 0; tid < nt; tid++)
	{
		args[tid].hashmap = &hashmap;
		args[tid].filename = &(*it);
		it++;
		pthread_create(&threads[tid], NULL, count_words_Thread, (void *)&args[tid]);	
	}
	for(int i = 0; i < nt; i++)
	{
		pthread_join(threads[i], NULL);
	}

	return hashmap;
}

void * ConcurrentHashMap::process_files_Thread(void *args)
{
	lockNFileNMap *arg = (lockNFileNMap *) args;
	for (int l = 0; l < arg->file_names->size(); l++)
	{
		if(pthread_mutex_trylock(&(arg->file_locks[l])) == 0){
			fileNMap args2;
			args2.hashmap = arg->hashmap;

			//Me posiciono en el l-esimo elemento de la lista
			std::list<std::string>::iterator it=arg->file_names->begin();
			
			int lfin = 0;
			while(lfin<l){it++;lfin++;}

 			args2.filename = &(*it);
			count_words_Thread(&args2);
		}
	}

}

ConcurrentHashMap ConcurrentHashMap::count_words(unsigned int n,std::list<std::string> archivos){
	ConcurrentHashMap  hashmap;
	pthread_t threads[n];
	pthread_mutex_t file_lock_list[archivos.size()];
	int tid;
	lockNFileNMap args;
	args.hashmap = &hashmap;
	args.file_names = &archivos;
	args.file_locks = file_lock_list;

	//Inicializo los locks de files
	for(int i = 0; i < archivos.size(); i++)
	{
		pthread_mutex_init(&file_lock_list[i], NULL);
	}

	//Asigno trabajo a los threads
	for(tid = 0; tid < n; tid++)
	{
		pthread_create(&threads[tid], NULL, process_files_Thread, (void *)&args);	
	}

	for(int i = 0; i < n; i++)
	{
		pthread_join(threads[i], NULL);
	}

	for(int i = 0; i < archivos.size(); i++)
	{
		pthread_mutex_destroy(&file_lock_list[i]);
	}
	return hashmap;
}

void * ConcurrentHashMap::readFilesThread(void *args)
{
	lockNFileNMap *arg = (lockNFileNMap *)args;

	
	for (int l = 0; l < arg->file_names->size(); l++)
	{		
		if(pthread_mutex_trylock(&(arg->file_locks[l])) == 0)
		{

			std::list<std::string>::iterator it=arg->file_names->begin();
			int lfin = 0;
			while(lfin<l){it++;lfin++;}
			// Paja tener que copiar algo que se devuelve por copia. No puedo tomar el puntero a la copia devuelta ya que es temporal.
			ConcurrentHashMap cm = count_words(*it);
			arg->hashmap->merge(cm);
		}
	}
}
tupla ConcurrentHashMap::maximums_sin_concurrencia(unsigned int p_archivos, unsigned int p_maximos, std::list<std::string> archs)
{
	pthread_t threads[p_archivos];
	int tid;
	pthread_mutex_t file_lock_list[archs.size()];
	lockNFileNMap args;
	ConcurrentHashMap total;
	//Creo tantas estructuras como archivos
	for(int i = 0; i < archs.size(); i++)
	{
		pthread_mutex_init(&file_lock_list[i], NULL);
	}

	args.hashmap = &total;
	args.file_names = &archs;
	args.file_locks = file_lock_list;
	
	//Creo los threads
	for(tid = 0; tid < p_archivos; tid++)
	{
		pthread_create(&threads[tid], NULL, readFilesThread, (void *)&args);	
	}
	//Espero su finalizacion
	for(int i = 0; i < p_archivos; i++)
	{
		pthread_join(threads[i], NULL);
	}
 	return total.maximum(p_maximos);
 }

 void ConcurrentHashMap::merge(ConcurrentHashMap& cm){
 	for(int t = 0; t < 26; t++)
 	{
 		Lista<tupla>::Iterador it = cm.tabla[t].CrearIt();
 		while(it.HaySiguiente())
 		{
 			tupla tup = it.Siguiente();
			unsigned int index = hash(tup.first);
			pthread_mutex_lock(&lock_list[index]);
			Lista< tupla >::Iterador it2 = tabla[index].CrearIt();
			bool insert = false;
			while(it2.HaySiguiente())
			{
				if(tup.first.compare(it2.Siguiente().first) == 0)
				{
					it2.Siguiente().second += tup.second;
					insert = true;
				}
				it2.Avanzar();
			}
			if(!insert)
			{
				tabla[index].push_front(tup);
			}
			pthread_mutex_unlock(&lock_list[index]);
			it.Avanzar();
 		}
 	}
 }

tupla ConcurrentHashMap::concurrent_maximum(unsigned int p_archivos, unsigned int p_maximos, std::list<std::string> archs){
	ConcurrentHashMap  hashmap = count_words(p_archivos, archs);
	return hashmap.maximum(p_maximos);
}
