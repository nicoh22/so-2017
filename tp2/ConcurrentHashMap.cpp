#include "ConcurrentHashMap.hpp"
#include <atomic>


ConcurrentHashMap::ConcurrentHashMap()
{}


ConcurrentHashMap::~ConcurrentHashMap()
{}

void ConcurrentHashMap::addAndInc(string key)
{
	unsigned int index = hash(key);
	Lista< pair<string, unsigned int> >::Iterator it = map[index].CrearIt();
	while(it.HaySiguiente())
	{
		if(strncmp(key, it.Siguiente().first ))
		{
			it.Siguiente()->second.getAndInc();
			return;
		}
		it.Avanzar();
	}



}


bool ConcurrentHashMap::member(string key)
{
	return false;
}

pair<string, unsigned int> ConcurrentHashMap::maximum(unsigned int nt)
{}


unsigned int ConcurrentHashMap::hash(string key)
{
	return key[0] - 97;
}
