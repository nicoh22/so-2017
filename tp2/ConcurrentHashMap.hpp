#include <pair>
#include <string>

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
}
