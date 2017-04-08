#ifndef __SCHED_NOMISTERY__
#define __SCHED_NOMISTERY__

#include <vector>
#include "basesched.h"
struct Task 
{
	int pid;
//	int prior;
	int cpu;
};

class SchedNoMistery : public SchedBase {
	public:
		SchedNoMistery(std::vector<int> argn);
        ~SchedNoMistery();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		// Completar
		std::vector<Task> tasks;
		int nextTask();
		void pop();
		void sort();
};

#endif
