#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	// Cada core tiene un quantum distinto
	for(int i = 2; i < argn.size(); i++)
	{
		quantum.push_back(argn(i));
		remaining.push_back(argn[i]);
	}
}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {
	tasks.push(pid);
}

void SchedRR::unblock(int pid) 
{
	load(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {

	int siguiente;
	if(current_pid(cpu) == IDLE_TASK)
	{
		siguiente = nextTask();
		return siguiente;
	}
	if(m == TICK)
	{
		remaining[cpu]--;
		if(remaining[cpu] == 0)
		{
			remaining[cpu] = quantum[cpu];
			if(!tasks.empty)
			{
				siguiente = q.front();
				q.pop();
				tasks.push(current_pid(cpu));
			}
			else
			{
				siguiente = IDLE_TASK;
			}
		}
		else
		{
			siguiente = current_pid(cpu);
		}

		return siguiente;
	}

}

int nextTask() //podria estar piola esta abstraccion
{
	int siguiente;
	if (!tasks.empty())
	{
		siguiente = tasks.front();
		tasks.pop();
	}
	else
	{
		siguiente = IDLE_TASK;
	}	
	return siguiente;
}
