#include "tasks.h"
#include <stdlib.h>

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}


// Ejercicio 1
// params: n, bmin, bmax, donde n=cant llamadas bloqueantes
void TaskConsola(int pid, vector<int> params)
{
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];

	time_t t;
	srand((unsigned) time(&t));

	//assert(bmin < bmax);

	for(int i = 0; i < n; i++)
	{
		uso_IO(pid, (rand() % (bmax - bmin + 1)) + bmin);// llamada bloqueante
	}
	return;//Lo pongo para ilustrar mejor que toma tiempo de cpu
}

// Ejercicio 3
// params: cantidad_repeticiones, tiempo_cpu, tiempo_bloqueo
void TaskPajarillo(int pid, vector<int> params)
{
	int cantidad_repeticiones = params[0];
   	int tiempo_cpu = params[1];
	int tiempo_bloqueo = params[2];
	for(int i = 0; i < cantidad_repeticiones; i++)
	{
		uso_CPU(pid, tiempo_cpu);
		uso_IO(pid, tiempo_bloqueo);
	}
	return;
}

void TaskPriorizada(int pid, vector<int> params) { 
	int tiempo_cpu = params[1];
	uso_CPU(pid, tiempo_cpu);
	return;
}

void TaskPrioBloqueo(int pid, vector<int> params) {
	int tiempo_cpu = params[1];
	int tiempo_bloqueo = params[2];

	uso_CPU(pid, tiempo_cpu);
	uso_IO(pid, tiempo_bloqueo);

	return;
}
void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskPajarillo, 3);
	register_task(TaskPriorizada, 2);
	register_task(TaskPrioBloqueo, 3);
}

