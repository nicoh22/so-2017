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
	ConcurrentHashMap *hashMap;
	std::string *filename;
} fileNMap;

ConcurrentHashMap::ConcurrentHashMap()
{
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_init(&lock_list[i], NULL);
	}
}


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
	Lista< tupla >::Iterador it = map[index].CrearIt();
	while(it.HaySiguiente())
	{
		if(key.compare(it.Siguiente().first) == 0)
		{
			it.Siguiente().second++;
			return;
		}
		it.Avanzar();
	}
	map[index].push_front(make_pair(key, 1));
	pthread_mutex_unlock(&lock_list[index]);
}


bool ConcurrentHashMap::member(std::string key)
{
	unsigned int index = hash(key);
	Lista< tupla  >::Iterador it = map[index].CrearIt();
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
		if(pthread_mutex_trylock(&(arg->locks[i])))
		{
			Lista< tupla >::Iterador it = map[i].CrearIt();
			//esta es la unica forma de saber si ListaAtomica esta vacia
			if(arg->maximums[i] == NULL && !(it.HaySiguiente()))
			{
				tupla max = std::make_pair("", 0);
				while(it.HaySiguiente())
				{
					tupla *actual = &it.Siguiente();
					if (actual->second > max.second) {
						max = *actual;
					}
					it.Avanzar();
				}

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

static ConcurrentHashMap count_words(std::string archivo){
	//Hay que devolver un ConcurrentHashMap, no un puntero
	//O sea que hay que implementar el constructor por copia 
	//declarar en stack el hash map y devolverlo
    std::ifstream myfile(archivo);
    ConcurrentHashMap *hashmap = new ConcurrentHashMap();
    if (myfile.is_open())
	{
		std::string word;
	    while (myfile >> word )
		{
			hashmap.push_front(word);
	    }
	}
	myfile.close();
	return hashmap;

};

void *ConcurrentHashMap::count_words_Thread(void *args)
{
	fileNMap *arg = (fileNMap *) args;
	std::ifstream myfile(arg->filename);
	if (myfile.is_open())
	{
		std::string word;
	    while (myfile >> word )
		{
			arg->hashmap.push_front(word);
	    }
	}
	myfile.close();

}

static ConcurrentHashMap count_words(std::list<string> archivos){
	ConcurrentHashMap hashmap = new ConcurrentHashMap();
	std::list<int>::iterator it=archivos.begin();
	pthread_t threads[archivos.size()];
	int tid;
	fileNMap args[archivos.size()];
	
	for(tid = 0; tid < nt; tid++)
	{
		args[tid]->hashmap = hashmap;
		args[tid]->filename = *it;
		it++;
		pthread_create(&threads[tid], NULL, &ConcurrentHashMap::count_words_Thread, (void *)&args[tid]);	
	}
	for(int i = 0; i < nt; i++)
	{
		pthread_join(threads[i], NULL);
	}

	return hashmap;
}
