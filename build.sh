#!build/sh

g++ -pedantic -Wall -std=c++17 -c -o out/main.o src/main.cc
g++ -pedantic -Wall -std=c++17 -c -o out/stack.o src/stack.cc
g++ -pedantic -Wall -std=c++17 -c -o out/operations.o src/operations.cc
g++ -pedantic -Wall -std=c++17 -c -o out/dict_operations.o src/dict_operations.cc

g++ -pedantic -Wall -std=c++17 -lm -o bin/main out/main.o out/stack.o out/operations.o out/dict_operations.o  


./bin/main

