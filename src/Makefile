# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = clang++
CFLAGS = -Wall -g

all: main test

test: test.o
	$(CC) $(CFLAGS) -o test test.o
	
test.o: test.cpp hashmap.hpp entry.hpp hashes.hpp
	$(CC) $(CFLAGS) -c test.cpp

main: main.o
	$(CC) $(CFLAGS) -o main main.o

main.o: main.cpp hashmap.hpp entry.hpp hashes.hpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm main.o main test.o test
