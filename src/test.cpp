#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <math.h>   
#include "hashmap.hpp"
using namespace std;

const size_t SMALL_SIZE = pow(2, 4);
const size_t MEDIUM_SIZE = pow(2, 10);
const size_t LARGE_SIZE = pow(2, 16);

void test_put(){
    HashMap<int, int, IntHash<int> > hmap(MEDIUM_SIZE);
    std::vector<int> nums;
    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        nums.push_back(i*i);
        hmap.put(i*i, i*i);
    }
    int value;
    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        assert(hmap.get(i*i, value));
        assert(value == i*i);
        
        assert(!hmap.get((i*i)-2, value));
    }
    cout << "Put Test Pass!" << endl;
}

void test_duplicate_keys(){
    HashMap<int, int, IntHash<int> > hmap(MEDIUM_SIZE);
    std::vector<int> nums;
    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        nums.push_back(i*i);
        hmap.put(i*i, i*i);
    }
    int value;
    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        assert(hmap.get(i*i, value));
        assert(value == i*i);
        
        assert(!hmap.get((i*i)-2, value));
    }

    cout << "Put Test Pass!" << endl;

}

void test_reallocation(){
    HashMap<int, int, IntHash<int> > hmap(SMALL_SIZE);
    std::vector<int> nums;

    int value;
    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        nums.push_back(i*i);
        hmap.put(i*i, i*i);
    }

    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        assert(hmap.get(i*i, value));
        assert(value == i*i);
        
        assert(!hmap.get((i*i)-2, value));
    }

    cout << "Reallocation Test Pass!" << endl;
}

int main () {
    test_put();
    test_reallocation();
}