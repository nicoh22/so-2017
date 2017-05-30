.POSIX:
.SUFFIXES:
.SUFFIXES: .o .cpp

CXX = g++
CXXFLAGS = -g -std=c++11
LDLIBS = -lrt -lpthread

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

BIN = test-1 test-2 test-3 test-4 test-5 test-6 test-maximums
OBJ = ConcurrentHashMap.o

all: $(BIN)

test-1: $(OBJ) test-1.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-1.cpp $(OBJ) $(LDLIBS)
	
test-1-run: test-1
	cat corpus | sort >corpus-post
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do ./test-1 $$((i + 1)) | sort | diff -u - corpus-post; done
	rm -f corpus-post corpus-[0-4]


test-2: $(OBJ) test-2.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-2.cpp $(OBJ) $(LDLIBS)
	
test-2-run: test-2
	awk -f corpus.awk corpus | sort >corpus-post
	./test-2 | sort | diff -u - corpus-post
	rm -f corpus-post

test-3: $(OBJ) test-3.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-3.cpp $(OBJ) $(LDLIBS)

test-3-run: test-3
	awk -f corpus.awk corpus | sort >corpus-post
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do ./test-3 | sort | diff -u - corpus-post; done
	rm -f corpus-post corpus-[0-4]

test-4: $(OBJ) test-4.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-4.cpp $(OBJ) $(LDLIBS)

test-4-run: test-4
	awk -f corpus.awk corpus | sort >corpus-post
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do ./test-4 $$((i + 1)) | sort | diff -u - corpus-post; done
	rm -f corpus-post corpus-[0-4]

test-5: $(OBJ) test-5.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-5.cpp $(OBJ) $(LDLIBS)

test-5-run: test-5
	awk -f corpus.awk corpus | sort -nk 2 | tail -n 1 >corpus-max
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-5 $$((i + 1)) $$((j + 1)) | diff -u - corpus-max; \
	done; done
	rm -f corpus-max corpus-[0-4]

test-6: $(OBJ) test-6.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-6.cpp $(OBJ) $(LDLIBS)

test-6-run: test-6
	awk -f corpus.awk corpus | sort -nk 2 | tail -n 1 >corpus-max
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-6 $$((i + 1)) $$((j + 1)) | diff -u - corpus-max; \
	done; done
	rm -f corpus-max corpus-[0-4]

test-maximums: test-maximums.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-maximums.cpp $(OBJ) $(LDLIBS)

test-maximums-run: test-maximums
	awk -f corpus.awk corpus | sort -nk 2 | tail -n 1 >corpus-max
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	printf "p_archivos, p_maximos, maximo concurrente, maximo no concurrente\n" > results.csv
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-maximums $$((i + 1)) $$((j + 1)) >> results.csv ; \
	done; done
	rm -f corpus-max corpus-[0-4]


test-equal: test-equal.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-equal.cpp $(OBJ) $(LDLIBS)

test-equal-run: test-equal
	printf "p_archivos, p_maximos, maximo concurrente, maximo no concurrente\n" > results.csv
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-equal $$((i + 1)) $$((j + 1)) >> results.csv ; \
	done; done
test-max-dif: test-max-dif.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-max-dif.cpp $(OBJ) $(LDLIBS)

test-max-dif-run: test-max-dif
	printf "p_archivos, p_maximos, maximo concurrente, maximo no concurrente\n" > results.csv
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-max-dif $$((i + 1)) $$((j + 1)) >> results.csv ; \
	done; done
test-max-prom: test-max-prom.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-max-prom.cpp $(OBJ) $(LDLIBS)

test-max-prom-run: test-max-prom
	printf "p_archivos, p_maximos, maximo concurrente, maximo no concurrente\n" > results.csv
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-max-prom $$((i + 1)) $$((j + 1)) >> results.csv ; \
	done; done
clean:
	rm -f $(BIN) $(OBJ)
	rm -f corpus-*
