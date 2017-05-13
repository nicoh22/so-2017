#include <pair>
#include <string>
#include "ListaAtomica.hpp"
#include <atomic>
#include <pthread>
typedef pair<string, unsigned int> tupla;
class ConcurrentHashMap()
{
	public:
		ConcurrentHashMap();
		~ConcurrentHashMap();
		void addAndInc(string key);
		bool member(string key);
		tupla maximum(unsigned int nt);
	private:
		//TODO
		Lista< tupla > map[26];
		pthread_mutex_t lock_list[26];
		unsigned int hash(string key);
}
