#include <vector>
#include <iterator>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"
#include <iostream>
#include <algorithm>
#include <iostream>

using namespace std;

//bool porPrior (Task a,Task b) { return (a.prior < b.prior); }
bool porCpu (Task a,Task b) { return (a.cpu < b.cpu); }

SchedNoMistery::SchedNoMistery(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	// Cada core tiene un quantum distinto
	/*
	cout << argn[0] << "\n";
	int cores = argn[1]; //creo que argn[0] tiene el nombre del scheduler
	for(int i = 2; i < cores; i++)
	{
		quantum.push_back(argn[i]);
		remaining.push_back(argn[i]);
	}
	*/
}

SchedNoMistery::~SchedNoMistery() {

}

void SchedNoMistery::load(int pid) {
	Task t;
	
	t.pid = pid;	

	std::vector<int> *params = tsk_params(pid);
	
//	t.prior = (*params)[0];
	t.cpu = (*params)[0];

	tasks.push_back(t);
	sort();
}

void SchedNoMistery::unblock(int pid) 
{
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
	int siguiente;
	if(tasks.empty())
	{
		switch(m)
		{
			case BLOCK: 
				siguiente = current_pid(cpu);
				break;
			case EXIT:
				siguiente = IDLE_TASK;
				break;
			case TICK:
				siguiente = current_pid(cpu);
				break;
			default:
				break;
		}
	}
	else
	{
		switch(m)
		{
			case BLOCK:
				siguiente = current_pid(cpu);
				break;
			case EXIT:
				siguiente = nextTask();
				break;
			case TICK:
				if(current_pid(cpu) == IDLE_TASK)
				{
					siguiente = nextTask();
				}
				else
				{
					siguiente = current_pid(cpu);
				}
				break;
			default:
				break;
		}
	}

	return siguiente;  
}

int SchedNoMistery::nextTask()
{
	Task t = tasks.front();
	pop();
	return t.pid;
}

void SchedNoMistery::pop() {
	tasks.erase(tasks.begin());
}

void SchedNoMistery::sort() {
	std::sort(tasks.begin(), tasks.end(), porCpu);
	//std::stable_sort(tasks.begin(), tasks.end(), porPrior);
}
