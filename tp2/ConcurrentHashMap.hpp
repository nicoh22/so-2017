#ifndef CONCURRENT_HASH_MAP_H__
#define CONCURRENT_HASH_MAP_H__
#include <utility>
#include <string>
#include "ListaAtomica.hpp"
#include <atomic>
#include <list>
//#include <pthread>
//using namespace std; 
typedef std::pair<std::string, unsigned int> tupla;

//pthread_mutex_t count_words_lock = PTHREAD_MUTEX_INITIALIZER;
class ConcurrentHashMap
{
	public:
		ConcurrentHashMap();
		//ConcurrentHashMap(const ConcurrentHashMap& other);
		~ConcurrentHashMap();
		void addAndInc(std::string key);
		bool member(std::string key);
		tupla maximum(unsigned int nt);

		static ConcurrentHashMap count_words(std::string archivo);
		static ConcurrentHashMap count_words(std::list<std::string> archivo);
        static ConcurrentHashMap count_words(unsigned int n,std::list<std::string> archivos);
        //static tupla maximum(unsigned int p_archivos, unsigned int p_maximos, list<string> archs);
        static tupla concurrent_maximum(unsigned int p_archivos, unsigned int p_maximos, std::list<std::string> archs);
		
		Lista< tupla > tabla[26];
	private:
		pthread_mutex_t lock_list[26];

		unsigned int hash(std::string key);
		static void *maxThread(void *args);
		static void *count_words_Thread(void *args);
		static void *process_files_Thread(void *args);
		void findMaximums(void * args);
};

#endif /* CONCURRENT_HASH_MAP_H__ */
