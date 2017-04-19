export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb 
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_fcfs.o sched_rr.o sched_psjf.o sched_no_mistery.o
MAIN=simusched
GRAPH=./graphsched.py

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS) sched_mistery.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h sched_rr.h sched_psjf.h sched_mistery.h sched_no_mistery.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

clean:
	rm -f $(OBJS) $(MAIN)

new: clean all

ejercicio1: all
	./$(MAIN) ej1/lote.tsk 1 0 5 SchedFCFS | $(GRAPH) > ej1/ej1.png

ejercicio2: all
	for CORES in 1 2 3 ; do \
		./$(MAIN) ej2/loteFCFS.tsk $$CORES 2 0 SchedFCFS | $(GRAPH) > ej2/$$COREScore.png ; \
	done

ejercicio3: all
	./$(MAIN) ej3/lote.tsk 2 2 0 SchedFCFS | $(GRAPH) > ej3/2cpu.png 
	./$(MAIN) ej3/lote.tsk 3 2 0 SchedFCFS | $(GRAPH) > ej3/3cpu.png 

ejercicio4: all
	./$(MAIN) ej4/loteMasCpuMenosBloqueo2.tsk 1 0 0 SchedRR 1 2 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo1002b.png
	./$(MAIN) ej4/loteMasCpuMenosBloqueo2.tsk 2 0 0 SchedRR 2 2 2 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo2002b.png
	./$(MAIN) ej4/loteMasCpuMenosBloqueo2.tsk 3 0 0 SchedRR 3 2 2 2 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo3002b.png
	./$(MAIN) ej4/loteMasCpuMenosBloqueo2.tsk 4 0 0 SchedRR 4 2 2 2 2 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo4002b.png

ejercicio5: all
	for CORES in 1 2 ; do \
		for LOTE in DistintasMayor DistintasMenor LargaLuegoCortas ; do \
			./$(MAIN) ej5/$$LOTE.tsk $$CORES 2 0 SchedMistery | $(GRAPH) > ej2/$$COREScores$$LOTE.png ; \
		done \
	done

ejercicio6: all

ejercicio7: all
