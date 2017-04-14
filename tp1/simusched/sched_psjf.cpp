#include <vector>
#include <iterator>
#include <queue>
#include "sched_psjf.h"
#include "basesched.h"
#include <iostream>
#include <algorithm>
#include <iostream>

using namespace std;

bool porPrior (Tuple a,Tuple b) { return (a.prior < b.prior); }
bool porCpu (Tuple a,Tuple b) { return (a.cpu < b.cpu); }

SchedPSJF::SchedPSJF(vector<int> argn) {
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

SchedPSJF::~SchedPSJF() {

}

void SchedPSJF::load(int pid) {
	Tuple t;
	
	t.pid = pid;	

	std::vector<int> *params = tsk_params(pid);
	
	t.prior = (*params)[0];
	t.cpu = (*params)[1];

	tasks.push_back(t);
	sort();
}

void SchedPSJF::unblock(int pid) 
{
	load(pid);
}

int SchedPSJF::tick(int cpu, const enum Motivo m) {
	int siguiente;
	if(tasks.empty())
	{
		switch(m)
		{
			case BLOCK: 
				siguiente = IDLE_TASK;
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
				siguiente = nextTask();
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
					int actual = current_pid(cpu);
					Tuple t = tasks.front();

					vector<int> *params = tsk_params(actual);

					// MARK: Mas bajo mas prioritario
					if (t.prior < (*params)[0]) {
						pop();
						siguiente = t.pid;
						load(actual);
					}else {
						if (t.prior == (*params)[0]) {
							if (t.cpu < (*params)[1]) {
								pop();
								siguiente = t.pid;
								load(actual);
							}else {
								siguiente = actual;
							}
						}else {
							siguiente = actual;
						}
					}	
				}
				break;
			default:
				break;
		}
	}

	return siguiente;
}

int SchedPSJF::nextTask()
{
	Tuple t = tasks.front();
	pop();
	return t.pid;
}

void SchedPSJF::pop() {
	tasks.erase(tasks.begin());
}

void SchedPSJF::sort() {
	std::stable_sort(tasks.begin(), tasks.end(), porCpu);
	std::stable_sort(tasks.begin(), tasks.end(), porPrior);
}
