# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -g -std=c++11 
INC = -I/Users/aqho/Desktop/PersonalCodingProjects/HashMap/include

all: bin/benchmarking bin/test_correctness_linear bin/test_correctness_linear2 bin/test_correctness_robinhood

test:
	./bin/test_correctness_linear
	./bin/test_correctness_linear2
	./bin/test_correctness_robinhood
	./bin/benchmarking

bin/benchmarking: out/benchmarking.o
	$(CC) $(CFLAGS) $^ -o $@ 
	
out/benchmarking.o: tests/benchmarking.cpp 
	$(CC) $(CFLAGS) $(INC) $^ -c -o $@ 

bin/test_correctness_linear: out/test_correctness_linear.o
	$(CC) $(CFLAGS) $^ -o $@
	
out/test_correctness_linear.o: tests/test_correctness_linear.cpp 
	$(CC) $(CFLAGS) $(INC) $^ -c -o $@ 

bin/test_correctness_linear2: out/test_correctness_linear2.o
	$(CC) $(CFLAGS) $^ -o $@
	
out/test_correctness_linear2.o: tests/test_correctness_linear2.cpp 
	$(CC) $(CFLAGS) $(INC) $^ -c -o $@ 

bin/test_correctness_robinhood: out/test_correctness_robinhood.o
	$(CC) $(CFLAGS) $^ -o $@
	
out/test_correctness_robinhood.o: tests/test_correctness_robinhood.cpp 
	$(CC) $(CFLAGS) $(INC) $^ -c -o $@ 

clean_all: clean_benchmarks clean_objs

clean_benchmarks:
	find bin/ -type f -not -name 'benchmarks.ipynb' -delete

clean_objs: 
	find out/ -type f -delete
