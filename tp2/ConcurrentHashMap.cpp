#include "ConcurrentHashMap.hpp"
#include <atomic>
#include <pthread>

typedef struct max_t{

	tupla *maximums[26];
	pthread_mutex_t locks[26];
	max_t()
	{
		for(int i = 0; i < 26; i++)
		{
			maximums[i] = NULL;
			pthread_mutex_init(locks[i], NULL);
		}
	}
} maxtarg;


ConcurrentHashMap::ConcurrentHashMap()
{
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_init(lock_list[i], NULL);
	}
}


ConcurrentHashMap::~ConcurrentHashMap()
{
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_destroy(lock_list[i]);
	}
}

void ConcurrentHashMap::addAndInc(string key)
{
	unsigned int index = hash(key);
	pthread_mutex_lock(lock_list[index]);
	Lista< tupla >::Iterator it = map[index].CrearIt();
	while(it.HaySiguiente())
	{
		if(strncmp(key, it.Siguiente()->first ))
		{
			it.Siguiente()->second++;
			return;
		}
		it.Avanzar();
	}
	map[index].push_front(key, 1);
	pthread_mutex_unlock(lock_list[index]);
}


bool ConcurrentHashMap::member(string key)
{
	unsigned int index = hash(key);
	Lista< pair<string, atomic<unsigned int> > >::Iterator it = map[index].CrearIt();
	while(it.HaySiguiente())
	{
		if(strncmp(key, it.Siguiente()->first ))
		{
			return true;
		}
		it.Avanzar();
	}
	return false;
}

void findMaximums(void *args)
{
	maxtarg *arg = (maxtargs *) args;
	for(int i = 0; i < 26; i++)
	{
		if(pthread_mutex_trylock(arg->locks[i]))
		{
			if(arg->maximums[i] == NULL && this->map[i] != NULL)
			{
				Lista< tupla >::Iterator it = map[i].CrearIt();
				tupla max = tupla("", 0);
				while(it.HaySiguiente())
				{
					tupla *actual = it.Siguiente();
					if (actual->second > max.second) {
						max = *actual;
					}
					it.Avanzar();
				}

				arg->maximums[i] = new pair(max);
			}

			pthread_mutex_unlock(arg->locks[i]);
		}
	}
}

tupla ConcurrentHashMap::maximum(unsigned int nt)
{
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_lock(lock_list[i]);
	}
	pthread_t threads[nt];
	int tid;
	maxtarg args;
	for(tid = 0; tid < nt; tid++)
	{
		pthread_create(&threads[tid], NULL, findMaximums, &args);	
	}
	for(int i = 0; i < nt; i++)
	{
		pthread_join(threads[i], NULL);
	}
	tupla max = tupla("", 0);
	for(int i = 0; i < 26; i++)
	{
		if(maxtarg.maximums[i] != NULL && maxtarg.maximums[i].second > max.second)
		{
			max = maxtarg.maximums[i];
		}
	}
	for(int i = 0; i < 26; i++)
	{
		pthread_mutex_unlock(lock_list[i]);
	}
	//TODO: destruir locks de maxtargs, funcion del struct.
	return max;
}


unsigned int ConcurrentHashMap::hash(string key)
{
	return key[0] - 97;
}
