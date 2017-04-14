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
	./$(MAIN) lote.tsk 1 0 5 SchedFCFS | $(GRAPH) > ej1/ej1.png

ejercicio2: all
	for CORES in 1 2 3 ; do \
		./$(MAIN) ej2/loteFCFS.tsk $$CORES 2 0 SchedFCFS | $(GRAPH) > ej2/$$COREScore.png ; \
	done

#ejercicio3: 
ejercicio4: all
	./$(MAIN) ej4/loteMasCpuMenosBloqueo.tsk 1 0 0 SchedRR 1 2 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo1001.png
	./$(MAIN) ej4/loteMasCpuMenosBloqueo.tsk 2 0 0 SchedRR 2 2 2 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo2001.png
	./$(MAIN) ej4/loteMasCpuMenosBloqueo.tsk 3 0 0 SchedRR 3 2 2 2 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo3001.png
	./$(MAIN) ej4/loteMasCpuMenosBloqueo.tsk 4 0 0 SchedRR 4 2 2 2 2 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo4001.png

#./$(MAIN) ej4/loteTodoIgual.tsk 1 0 0 SchedRR 1 1 | $(GRAPH) > ej4/imgTodoIgual1001.png
#./$(MAIN) ej4/loteTodoIgual.tsk 1 0 0 SchedRR 1 4 | $(GRAPH) > ej4/imgTodoIgual1004.png
#./$(MAIN) ej4/loteTodoIgual.tsk 2 0 0 SchedRR 2 1 1 | $(GRAPH) > ej4/imgTodoIgual2001.png
#./$(MAIN) ej4/loteTodoIgual.tsk 3 0 0 SchedRR 3 1 1 1 | $(GRAPH) > ej4/imgTodoIgual3001.png
#./$(MAIN) ej4/loteTodoIgual.tsk 4 0 0 SchedRR 4 1 1 1 1 | $(GRAPH) > ej4/imgTodoIgual4001.png
#./$(MAIN) ej4/loteTodoIgualCrec.tsk 1 0 0 SchedRR 1 1 | $(GRAPH) > ej4/imgTodoIgualCrec1001.png
#./$(MAIN) ej4/loteTodoIgualCrec.tsk 2 0 0 SchedRR 2 1 1 | $(GRAPH) > ej4/imgTodoIgualCrec2001.png
#./$(MAIN) ej4/loteTodoIgualCrec.tsk 3 0 0 SchedRR 3 1 1 1 | $(GRAPH) > ej4/imgTodoIgualCrec3001.png
#./$(MAIN) ej4/loteTodoIgualCrec.tsk 4 0 0 SchedRR 4 1 1 1 1 | $(GRAPH) > ej4/imgTodoIgualCrec4001.png
#./$(MAIN) ej4/loteMenosCpuMasBloqueo.tsk 1 0 0 SchedRR 1 1 | $(GRAPH) > ej4/imgMenosCpuMasBloqueo1001.png
#./$(MAIN) ej4/loteMenosCpuMasBloqueo.tsk 2 0 0 SchedRR 2 1 1 | $(GRAPH) > ej4/imgMenosCpuMasBloqueo2001.png
#./$(MAIN) ej4/loteMenosCpuMasBloqueo.tsk 3 0 0 SchedRR 3 1 1 1 | $(GRAPH) > ej4/imgMenosCpuMasBloqueo3001.png
#./$(MAIN) ej4/loteMenosCpuMasBloqueo.tsk 4 0 0 SchedRR 4 1 1 1 1 | $(GRAPH) > ej4/imgMenosCpuMasBloqueo4001.png
#./$(MAIN) ej4/loteTodoIgual.tsk 1 0 0 SchedRR 1 4 | $(GRAPH) > ej4/imgTodoIgual1004.png
#./$(MAIN) ej4/loteTodoIgualCrec.tsk 1 0 0 SchedRR 1 4 | $(GRAPH) > ej4/imgTodoIgualCrec1004.png
#./$(MAIN) ej4/loteMasCpuMenosBloqueo.tsk 1 0 0 SchedRR 1 4 | $(GRAPH) > ej4/imgMasCpuMenosBloqueo1004.png
#./$(MAIN) ej4/loteMenosCpuMasBloqueo.tsk 1 0 0 SchedRR 1 4 | $(GRAPH) > ej4/imgMenosCpuMasBloqueo1004.png
ejercicio5: all
	for CORES in 1 2 ; do \
		for LOTE in DistintasMayor DistintasMenor LargaLuegoCortas ; do \
			./$(MAIN) ej5/$$LOTE.tsk $$CORES 2 0 SchedMistery | $(GRAPH) > ej2/$$COREScores$$LOTE.png ; \
		done \
	done

#ejercicio6: 
