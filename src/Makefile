# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g -std=c++11

all: test_speed test_correctness_linear test_correctness_robinhood

test:
	./test_correctness_linear
	./test_correctness_robinhood
	./test_speed

test_speed: test_speed.o
	$(CC) $(CFLAGS) -o test_speed test_speed.o
	
test_speed.o: test_speed.cpp hashmaps.hpp entry.hpp hashes.hpp
	$(CC) $(CFLAGS) -c test_speed.cpp

test_correctness_linear: test_correctness_linear.o
	$(CC) $(CFLAGS) -o test_correctness_linear test_correctness_linear.o
	
test_correctness_linear.o: test_correctness_linear.cpp linear_map.hpp entry.hpp hashes.hpp
	$(CC) $(CFLAGS) -c test_correctness_linear.cpp

test_correctness_robinhood: test_correctness_robinhood.o
	$(CC) $(CFLAGS) -o test_correctness_robinhood test_correctness_robinhood.o
	
test_correctness_robinhood.o: test_correctness_robinhood.cpp robinhood_map.hpp entry.hpp hashes.hpp
	$(CC) $(CFLAGS) -c test_correctness_robinhood.cpp

clean:
	find benchmarks ! -name benchmarks.ipynb -type f -delete 
	rm test_speed.o test_speed
	rm test_correctness_linear.o test_correctness_linear
	rm test_correctness_robinhood.o test_correctness_robinhood
