#include <time.h>
#include <iostream>
#include <cstdlib>
#include "ConcurrentHashMap.hpp"


using namespace std;

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

int main(int argc, char **argv) {
	pair<string, unsigned int> p;
	list<string> l = { "corpus-0", "corpus-1", "corpus-2", "corpus-3", "corpus-4" };

	if (argc != 3) 
	{
		cerr << "uso: " << argv[0] << " #tarchivos #tmaximum" << endl;
		return 1;
	}
	timespec maxcstart, maxcend, maxncstart, maxncend;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &maxncstart);
	p = ConcurrentHashMap::maximums_sin_concurrencia(atoi(argv[1]), atoi(argv[2]), l);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &maxncend);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &maxcstart);
	p = ConcurrentHashMap::concurrent_maximum(atoi(argv[1]), atoi(argv[2]), l);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &maxcend);
	//cout << p.first << " " << p.second << endl;
	cout<<diff(maxcstart,maxcend).tv_sec<<":"<<diff(maxcstart,maxcend).tv_nsec<<endl;
	cout<<diff(maxncstart,maxncend).tv_sec<<":"<<diff(maxncstart,maxncend).tv_nsec<<endl;
	return 0;
