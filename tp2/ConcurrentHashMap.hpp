#include <utility>
#include <string>
#include "ListaAtomica.hpp"
#include <atomic>
//#include <pthread>
//using namespace std; 
typedef std::pair<std::string, unsigned int> tupla;
class ConcurrentHashMap
{
	public:
		ConcurrentHashMap();
		~ConcurrentHashMap();
		void addAndInc(std::string key);
		bool member(std::string key);
		tupla maximum(unsigned int nt);

		static ConcurrentHashMap count_words(string archivo);
		static ConcurrentHashMap count_words(std::list<string> archivo);
        //ConcurrentHashMap count_words(unsigned int n,std::list<string> archivo);
        //tupla maximum(unsigned int p_archivos, unsigned int p_maximos, list<string> archs);
	private:
		Lista< tupla > map[26];
		pthread_mutex_t lock_list[26];
		unsigned int hash(std::string key);
		static void *maxThread(void *args);
		void findMaximums(void * args);
};
