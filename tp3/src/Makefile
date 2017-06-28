.PHONY : all clean run new tests

MPIROOT = /usr

CFLAGS+= -Wall -g $(INCL)
CC=gcc
MPICC=  $(MPIROOT)/bin/mpic++
MPIRUN= $(MPIROOT)/bin/mpirun
INCL= -I$(MPIROOT)/include
SRCS= consola.cpp main.cpp nodo.cpp HashMap.cpp
BIN= dist_hashmap

all: dist_hashmap

$(BIN): $(SRCS)
	$(MPICC) $(CFLAGS) -o $(BIN) $(SRCS)

clean:
	rm -f $(BIN)

new: clean all

tests: test1 test2 test3

test1: dist_hashmap
	cat tests/test1.test | $(MPIRUN) -host localhost -np 5 ./$(BIN) > tests/test1.out
	diff tests/test1.out tests/test1.expected

test2: dist_hashmap
	cat tests/test2.test | $(MPIRUN) -host localhost -np 3 ./$(BIN) > tests/test2.out
	diff tests/test2.out tests/test2.expected

test3: dist_hashmap
	cat tests/test3.test | $(MPIRUN) -host localhost -np 5 ./$(BIN) > tests/test3.out
	diff tests/test3.out tests/test3.expected
