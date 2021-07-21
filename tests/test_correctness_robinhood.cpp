#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <math.h>   
#include <unordered_map>
#include "robinhood_map.hpp"
using namespace std;

const size_t SMALL_SIZE = pow(2, 5);
const size_t MEDIUM_SIZE = pow(2, 10);
const size_t LARGE_SIZE = pow(2, 15);

const size_t MAX_ELEM = pow(2, 15);

bool is_equal(RobinhoodMap<int, int> &rmap, std::unordered_map<int, int> &umap){

    for (auto i : rmap){
        int key = i.getKey();
        int value = i.getValue();

        if(!umap.count(key)){
            cout << "ERROR: " << key << " not found in umap" << endl; 
        }
        assert(umap.count(key));
        if(!(umap.at(key) == value)){
            cout << "ERROR: " << umap.at(key) << " != " << value << endl; 
        }
        assert(umap.at(key) == value);
    }

    for (auto i: umap){
        int key = i.first;
        int value = i.second;

        int candidate_value;
        if(!rmap.emplace(key, candidate_value)){
            cout << "ERROR: " << key << " not found in rmap" << endl;
        }
        assert(rmap.emplace(key, candidate_value));
        
        if(!(candidate_value == value)){
            cout << "ERROR: " << candidate_value << " != " << value << endl; 
        }
        assert(candidate_value == value);
    }

    return true;
}

void test_insert(){
    RobinhoodMap<int, int> rmap(MEDIUM_SIZE);
    std::unordered_map<int, int> umap;
    
    for(int i = 0; i < 100; i++){
        rmap.insert(i*i, i);
        umap[i*i] = i;
    }

    assert(is_equal(rmap, umap));
    cout << "Put Test Pass!" << endl;
}

void test_duplicate_keys(){
    RobinhoodMap<int, int> rmap(MEDIUM_SIZE);
    std::unordered_map<int, int> umap;

    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        rmap.insert(i*i, i*i);
        umap[i*i] = i*i;
    }
    
    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        rmap.insert(i*i, i);
        umap[i*i] = i;
    }
    
    assert(is_equal(rmap, umap));
    cout << "Duplicate Key Test Pass!" << endl;
}

void test_reallocation(){
    RobinhoodMap<int, int> rmap(SMALL_SIZE);
    std::unordered_map<int, int> umap;

    for(int i = 0; i < MEDIUM_SIZE; i++){
        rmap.insert(i*i, i);
        umap[i*i] = i;
    }
    
    assert(is_equal(rmap, umap));
    cout << "Reallocation Test Pass!" << endl;
}

void test_remove(){
    RobinhoodMap<int, int> rmap(SMALL_SIZE);
    std::unordered_map<int, int> umap;
    std::vector<int> nums;
    
    for(int i = 0; i < MEDIUM_SIZE; i++){   
        rmap.insert(i*i, i*i);
        umap[i*i] = i*i;
        nums.push_back(i*i);
    }

    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        size_t idx = rand() % nums.size();
        int key = nums[idx];
        std::swap(nums[idx], nums[nums.size() - 1]);
        nums.pop_back();
        rmap.remove(key);
        umap.erase(key);
        assert(is_equal(rmap, umap));
    }

    for(int i = 0; i < MEDIUM_SIZE; i++){   
        int key = rand() % MEDIUM_SIZE/2;
        
        rmap.insert(key, 2*key);
        umap[key] = 2*key;
    }

    assert(is_equal(rmap, umap));

    cout << "Remove Test Pass!" << endl;
}

void test_interleaved1(){
    RobinhoodMap<int, int> rmap(SMALL_SIZE);
    std::unordered_map<int, int> umap;
    size_t count = 0;
    for(int i = 0; i < LARGE_SIZE; i++){
        int action = rand() % 100;
        int key = rand() % LARGE_SIZE;
        int value = rand() % LARGE_SIZE;

        if(action < 65){
            rmap.insert(key, value);
            umap[key] = value;    
        }
        else if(action < 90){
            rmap.remove(key);
            umap.erase(key);
        }
        else{
            int candidate_value = -1;
            assert(rmap.emplace(key, candidate_value) == umap.count(key));
            
            if(candidate_value != -1){
                assert(umap.at(key) == candidate_value);
            }
        }

        count++;

        if(count == 1000){
            assert(is_equal(rmap, umap));
            count = 0;
        }
    }
    
    assert(is_equal(rmap, umap));
    
    cout << "Interleaved Test 1 Pass!" << endl;
}

void test_interleaved2(){
    RobinhoodMap<int, int> rmap(SMALL_SIZE);
    std::unordered_map<int, int> umap;
    size_t count = 0;
    for(int i = 0; i < LARGE_SIZE; i++){
        int action = rand() % 100;
        int key = rand() % 100;
        int value = rand() % 100;

        if(action < 65){
            rmap.insert(key, value);
            umap[key] = value;    
        }
        else if(action < 90){
            rmap.remove(key);
            umap.erase(key);
        }
        else{
            int candidate_value = -1;
            assert(rmap.emplace(key, candidate_value) == umap.count(key));
            
            if(candidate_value != -1){
                assert(umap.at(key) == candidate_value);
            }
        }

        count++;

        if(count == 1000){
            assert(is_equal(rmap, umap));
            count = 0;
        }
    }
    
    assert(is_equal(rmap, umap));
    
    cout << "Interleaved Test 2 Pass!" << endl;
}

int main () {
    cout << "===========================" << endl;
    cout << "Starting RobinhoodMap Tests" << endl;
    cout << "===========================" << endl;
    test_insert();
    test_duplicate_keys();
    test_reallocation();
    test_remove();
    test_interleaved1();
    test_interleaved2();
}