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
		./$(MAIN) ej2/loteFCFS.tsk $$CORES 2 0 SchedFCFS | $(GRAPH) > ej2/core$$CORES.png ; \
	done

ejercicio3: all
	./$(MAIN) ej3/lote.tsk 2 2 0 SchedFCFS | $(GRAPH) > ej3/2cpu.png 
	./$(MAIN) ej3/lote.tsk 3 2 0 SchedFCFS | $(GRAPH) > ej3/3cpu.png 

ejercicio4: all
	./$(MAIN) ej4/loteSinBloqueo.tsk 1 0 0 SchedRR 1 2 | $(GRAPH) > ej4/imgSinBloqueo.png
	./$(MAIN) ej4/loteConBloqueo1.tsk 1 0 0 SchedRR 1 2 | $(GRAPH) > ej4/imgConBloqueo1.png
	./$(MAIN) ej4/loteConBloqueo2.tsk 1 0 0 SchedRR 1 2 | $(GRAPH) > ej4/imgConBloqueo2.png
	./$(MAIN) ej4/loteQuantumDiff.tsk 2 0 0 SchedRR 2 1 2 | $(GRAPH) > ej4/imgQuantumDiff.png

ejercicio5: all
	for CORES in 1 2 ; do \
		for LOTE in DistintasMayor DistintasMenor LargaLuegoCortas ; do \
			./$(MAIN) ej5/$$LOTE.tsk $$CORES 2 0 SchedMistery | $(GRAPH) > ej5/cores$$CORES$$LOTE.png ; \
		done \
	done

ejercicio6: all
	./$(MAIN) ascP-igualCPU.tsk 1 0 0 SchedPSJF | $(GRAPH) > ej6/tests/ascP-igualCPU.png
	./$(MAIN) desorden-tiemposCambiados.tsk 1 0 0 SchedPSJF | $(GRAPH) > ej6/tests/desorden-tiemposCambiados.png
	./$(MAIN) dscP-igualCPU.tsk 1 0 0 SchedPSJF | $(GRAPH) > ej6/tests/dscP-igualCPU.png
	./$(MAIN) igualP-ascCPU.tsk 1 0 0 SchedPSJF | $(GRAPH) > ej6/tests/igualP-ascCPU.png
	./$(MAIN) igualP-decCPU.tsk 1 0 0 SchedPSJF | $(GRAPH) > ej6/tests/igualP-decCPU.png
	./$(MAIN) igualP-rndCPU.tsk 1 0 0 SchedPSJF | $(GRAPH) > ej6/tests/igualP-rndCPU.png
	./$(MAIN) ordenInverso-cpu-prioridad.tsk 1 0 0 SchedPSJF | $(GRAPH) > ej6/tests/ordenInverso-cpu-prioridad.png
	./$(MAIN) rndP-igualCPU.tsk 1 0 0 SchedPSJF | $(GRAPH) > ej6/tests/rndP-igualCPU.png

ejercicio7: all
	for LOTE in MasCpuLargo MenosCpuLargo ; do \
		./$(MAIN) ej7/Largo/lote$$LOTE.tsk 1 1 0 SchedNoMistery | $(GRAPH) > ej7/Largo/imgNM1core$$LOTE.png ; \
		./$(MAIN) ej7/Largo/lote$$LOTE.tsk 2 1 0 SchedNoMistery | $(GRAPH) > ej7/Largo/imgNM2core$$LOTE.png ; \
		./$(MAIN) ej7/Largo/lote$$LOTE.tsk 1 1 0 SchedRR 1 5 | $(GRAPH) > ej7/Largo/imgRR1core$$LOTE.png ; \
		./$(MAIN) ej7/Largo/lote$$LOTE.tsk 2 1 0 SchedRR 2 5 5 | $(GRAPH) > ej7/Largo/imgRR2core$$LOTE.png ; \
	done 
	for CORES in 1 2 ; do \
		for LOTE in MasPrioMasCpuLargo MasPrioMenosCpuLargo MenosPrioMasCpuLargo MenosPrioMenosCpuLargo ; do \
			./$(MAIN) ej7/Largo/lote$$LOTE.tsk $$CORES 1 0 SchedPSJF | $(GRAPH) > ej7/Largo/imgPSJF$$LOTE$$CORES.png ; \
		done \
	done 
	for LOTE in MasCpuMasBloqueo MasCpuMenosBloqueo ; do \
		./$(MAIN) ej7/ConBloqueo/lote$$LOTE.tsk 1 1 0 SchedNoMistery | $(GRAPH) > ej7/ConBloqueo/imgNM1core$$LOTE.png ; \
		./$(MAIN) ej7/ConBloqueo/lote$$LOTE.tsk 2 1 0 SchedNoMistery | $(GRAPH) > ej7/ConBloqueo/imgNM2core$$LOTE.png ; \
		./$(MAIN) ej7/ConBloqueo/lote$$LOTE.tsk 1 1 0 SchedRR 1 5 | $(GRAPH) > ej7/ConBloqueo/imgRR1core$$LOTE.png ; \
		./$(MAIN) ej7/ConBloqueo/lote$$LOTE.tsk 2 1 0 SchedRR 2 5 5 | $(GRAPH) > ej7/ConBloqueo/imgRR2core$$LOTE.png ; \
	done 
	for CORES in 1 2 ; do \
		for LOTE in MenosPrioMasCpuMasBloqueo MenosPrioMasCpuMenosBloqueo ; do \
			./$(MAIN) ej7/ConBloqueo/lote$$LOTE.tsk $$CORES 1 0 SchedPSJF | $(GRAPH) > ej7/ConBloqueo/imgPSJF$$LOTE$$CORES.png ; \
		done \
	done 




