#ifndef __SCHED_PSJF__
#define __SCHED_PSJF__

#include <vector>
#include <queue>
#include "basesched.h"

struct Tuple {
	int pid;
	int prior;
	int cpu;
};

class SchedPSJF : public SchedBase {
	public:
		SchedPSJF(std::vector<int> argn);
        ~SchedPSJF();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		// Completar
		std::vector<Tuple> tasks;
		int nextTask();
		void pop();
		void sort();
};

#endif
