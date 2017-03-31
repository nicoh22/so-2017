#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	// Cada core tiene un quantum distinto
	int cores = argn[1]; //creo que argn[0] tiene el nombre del scheduler
	for(int i = 2; i < cores; i++)
	{
		quantum.push_back(argn[i]);
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
/*
	int siguiente;
	if(current_pid(cpu) == IDLE_TASK)
	{
		siguiente = nextTask();
		//Hay que fijarse si esta o no vacia la cola
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
				siguiente = tasks.front();
				tasks.pop();
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
	//Este tiene una falla: 
*/
	//Version 2
	if(tasks.empty)
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
				remaining[cpu]--;
				siguiente = current_pid(cpu);
				break;
			case default:
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
				remaining[cpu]--;
				if(remaining[cpu] == 0)
				{
					siguiente = nextTask();
					load(current_pid(cpu));//y si es la IDLE?
				}
				else
				{
					siguiente = current_pid(cpu);
				}
				break;
			case default:
				break;
		}
	}
	if(remaining[cpu] == 0)
	{
		remaining[cpu] = quantum[cpu];
	}
	//NOTA: Este scheduler si esta en idle va a completar 
	//el quantum antes de swapear con otra tarea (a lo orga 2)
	//Me parece igual que eso no es el comportamiento deseado.
	//Esa logica la deberia agregar bajo el case de TICK.

}

int nextTask()
{
	int siguiente = tasks.front();
	tasks.pop();
	return siguiente;
}
