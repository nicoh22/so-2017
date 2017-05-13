#include <pair>
#include <string>
#include "ListaAtomica.hpp"

class ConcurrentHashMap()
{
	public:
		ConcurrentHashMap();
		~ConcurrentHashMap();
		void addAndInc(string key);
		bool member(string key);
		pair<string, unsigned int> maximum(unsigned int nt);
	private:
		//TODO
		Lista< pair<string, unsigned int> > map[26];
		unsigned int hash(string key);
}
