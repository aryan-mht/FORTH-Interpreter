#!build/sh

g++ -pedantic -Wall -std=c++17 -g -c -o debug/main.o src/main.cc
g++ -pedantic -Wall -std=c++17 -g -c -o debug/stack.o src/stack.cc
g++ -pedantic -Wall -std=c++17 -g -c -o debug/operations.o src/operations.cc
g++ -pedantic -Wall -std=c++17 -g -c -o debug/dict_operations.o src/dict_operations.cc

g++ -pedantic -Wall -std=c++17 -lm -g -o bin/maing debug/main.o debug/stack.o debug/operations.o debug/dict_operations.o  