#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	// Cada core tiene un quantum distinto
	cout << argn[0] << "\n";
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
	//Version 1:
	int siguiente;
	if(current_pid(cpu) == IDLE_TASK)
	{
		if(tasks.empty())
		{
			siguiente = current_pid(cpu);
		}
		else
		{
			siguiente = nextTask();
		}
		return siguiente;
	}

	if(m == TICK)
	{
		remaining[cpu]--;
		if(remaining[cpu] == 0)
		{
			remaining[cpu] = quantum[cpu];
			if(!tasks.empty())
			{
				siguiente = nextTask(); 
				load(current_pid(cpu));
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
	else
	{ //BLOCK y EXIT
		if(tasks.empty())
		{
			return IDLE_TASK;
		}
		else
		{
			return nextTask();
		}
	}
	//Fin version 1
*/	
	//Version 2
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
				remaining[cpu]--;
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
				remaining[cpu]--;
				if(current_pid(cpu) == IDLE_TASK)
				{
					siguiente = nextTask();
				}
				else
				{
					if(remaining[cpu] == 0)
					{
						siguiente = nextTask();
						load(current_pid(cpu));
					}
					else
					{
						siguiente = current_pid(cpu);
					}
				}
				break;
			default:
				break;
		}
	}
	if(remaining[cpu] == 0)
	{
		remaining[cpu] = quantum[cpu];
	}
	return siguiente;
	//NOTA: Este scheduler si esta en idle va a completar 
	//el quantum antes de swapear con otra tarea (a lo orga 2)
	//Me parece igual que eso no es el comportamiento deseado.
	//Esa logica la deberia agregar bajo el case de TICK.
	// 1/4: Fixed

	//Nuevos problemas: 
}

int SchedRR::nextTask()
{
	int siguiente = tasks.front();
	tasks.pop();
	return siguiente;
}
