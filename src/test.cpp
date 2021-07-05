#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>

#include "hashmap.hpp"
using namespace std;

const size_t INIT_SIZE = 1024;

void test_put(){
    
    HashMap<int, int, IntHash<int> > hmap(INIT_SIZE);
    std::vector<int> nums;
    for(int i = 0; i < INIT_SIZE/2; i++){
        nums.push_back(i*i);
        hmap.put(i*i, i*i);
    }
    int value;
    for(int i = 0; i < INIT_SIZE/2; i++){
        assert(hmap.get(i*i, value));
        assert(value == i*i);
        
        bool res = hmap.get((i*i)-2, value);
        if(res){
            cout << (i*i) - 2 << ", " << value << endl;
        }
        assert(!hmap.get((i*i)-2, value));
    }

    cout << "Put Test Pass!" << endl;
}

int main () {
    test_put();
}