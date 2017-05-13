#include <pair>
#include <string>
#include "ListaAtomica.hpp"
#include <atomic>

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
		Lista< pair<string, atomic<unsigned int> > > map[26];
		unsigned int hash(string key);
}
