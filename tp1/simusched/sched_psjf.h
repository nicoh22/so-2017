#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"

struct Tuple {
	int pid;
	int prior;
	int cpu;
};

bool porPrior (Tuple a,Tuple b) { return (a.prior > b.prior); }
bool porCpu (Tuple a,Tuple b) { return (a.cpu > b.cpu); }

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
};

#endif
