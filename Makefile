CC = g++
CFLAGS = -Wall -pedantic -std=c++17
LIBS = -lm
RM = rm
TAR = tar

all: out/stack.o out/main.o out/operations.o out/dict_operations.o
	$(CC) $(CFLAGS) $(LIBS) -o bin/main out/stack.o out/main.o out/operations.o out/dict_operations.o

out/stack.o: src/stack.cc src/dict.h src/stack.h src/datum.h
	echo "Compiling stack.cc"
	$(CC) $(CFLAGS) -c src/stack.cc -o out/stack.o

out/main.o: src/main.cc src/dict.h src/stack.h src/datum.h
	echo "Compiling main.cc"
	$(CC) $(CFLAGS) -c -o out/main.o src/main.cc

out/operations.o: src/operations.cc src/dict.h src/stack.h src/datum.h
	echo "Compiling operations.cc"
	$(CC) $(CFLAGS) -c -o out/operations.o src/operations.cc

out/dict_operations.o: src/dict_operations.cc src/dict.h src/stack.h src/datum.h
	echo "Compiling dict_operations.cc"
	$(CC) $(CFLAGS) -c -o out/dict_operations.o src/dict_operations.cc

clean: 
	rm -i out/*.o

squeaky: clean
	echo "squeaky executing"
	ls -i bin/* *~

debugMode: debug
	bin/maing -d

debug: out/stack-g.o out/main-g.o out/operations-g.o out/dict_operations-g.o
	$(CC) $(CFLAGS)  $(LIBS) -g -o bin/maing out/main-g.o out/stack-g.o out/operations-g.o out/dict_operations-g.o  

out/stack-g.o: src/stack.cc src/dict.h src/stack.h src/datum.h
	$(CC) $(CFLAGS) -g -c -o out/stack-g.o src/stack.cc

out/main-g.o: src/main.cc src/dict.h src/stack.h src/datum.h
	$(CC) $(CFLAGS) -g -c -o out/main-g.o src/main.cc

out/operations-g.o: src/operations.cc src/dict.h src/stack.h src/datum.h
	$(CC) $(CFLAGS) -c -o out/operations-g.o src/operations.cc

out/dict_operations-g.o: src/dict_operations.cc src/dict.h src/stack.h src/datum.h
	$(CC) $(CFLAGS) -c -o out/dict_operations-g.o src/dict_operations.cc

