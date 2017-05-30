#include <time.h>
#include <iostream>
#include <cstdlib>
#include "ConcurrentHashMap.hpp"

#define NANOSEC_PER_SEC 1000000000
#define CORRIDAS 100
using namespace std;

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = NANOSEC_PER_SEC+end.tv_nsec-start.tv_nsec;
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
	long long tiempoConcurrente, tiempoNoConcurrente;
	long long sumaC = 0;
	long long sumaNC = 0;


	for(int i = 0; i < CORRIDAS; i++)
	{

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &maxncstart);
		p = ConcurrentHashMap::maximums_sin_concurrencia(atoi(argv[1]), atoi(argv[2]), l);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &maxncend);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &maxcstart);
		p = ConcurrentHashMap::concurrent_maximum(atoi(argv[1]), atoi(argv[2]), l);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &maxcend);
		
		//cout << p.first << " " << p.second << endl;
		timespec tspc = diff(maxcstart,maxcend);
		timespec tspnc = diff(maxncstart,maxncend);
		long long tiempoConcurrente = (tspc.tv_sec * NANOSEC_PER_SEC) + tspc.tv_nsec;
		long long tiempoNoConcurrente = (tspnc.tv_sec * NANOSEC_PER_SEC) + tspnc.tv_nsec;
		sumaC += tiempoConcurrente;
		sumaNC += tiempoNoConcurrente;


	}

	double promC = ( (double) sumaC )/ CORRIDAS;
	double promNC = ( (double) sumaNC )/ CORRIDAS;

	//csv?
	cout << argv[1] << ",";
	cout << argv[2] << ",";
	cout << promC << ",";
	cout << promNC << endl;


	return 0;
}
