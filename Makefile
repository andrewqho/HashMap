# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -g -std=c++11 
INC = -I/Users/aqho/Desktop/PersonalCodingProjects/HashTable/include

all: bin/test_speed bin/test_correctness_linear bin/test_correctness_robinhood

test:
	./bin/test_correctness_linear
	./bin/test_correctness_robinhood
	./bin/test_speed

bin/test_speed: out/test_speed.o
	$(CC) $(CFLAGS) $^ -o $@ 
	
out/test_speed.o: tests/test_speed.cpp 
	$(CC) $(CFLAGS) $(INC) $^ -c -o $@ 

bin/test_correctness_linear: out/test_correctness_linear.o
	$(CC) $(CFLAGS) $^ -o $@
	
out/test_correctness_linear.o: tests/test_correctness_linear.cpp 
	$(CC) $(CFLAGS) $(INC) $^ -c -o $@ 

bin/test_correctness_robinhood: out/test_correctness_robinhood.o
	$(CC) $(CFLAGS) $^ -o $@
	
out/test_correctness_robinhood.o: tests/test_correctness_robinhood.cpp 
	$(CC) $(CFLAGS) $(INC) $^ -c -o $@ 

clean:
	find bin/ -type f -not -name 'benchmarks.ipynb' -delete
	find out/ -type f -delete 
