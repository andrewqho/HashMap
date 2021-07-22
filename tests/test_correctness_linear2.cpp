#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <math.h>   
#include <unordered_map>
#include "linear_map2.hpp"
using namespace std;

const size_t SMALL_SIZE = pow(2, 5);
const size_t MEDIUM_SIZE = pow(2, 10);
const size_t LARGE_SIZE = pow(2, 15);

const size_t MAX_ELEM = pow(2, 10);

bool is_equal(LinearMap2<int, int> &lmap2, std::unordered_map<int, int> &umap){

    for (auto i : lmap2){
        int& key = i.getKey();
        int& value = i.getValue();

        if(umap.count(key) == 0){
            cout << key << " not found in umap" << endl;
        }
        assert(umap.count(key));
        assert(umap.at(key) == value);
    }

    for (auto i: umap){
        int key = i.first;
        int value = i.second;

        int candidate_value;
        assert(lmap2.emplace(key, candidate_value));
        assert(candidate_value == value);
    }

    return true;
}

void test_insert(){
    LinearMap2<int, int> lmap2(MEDIUM_SIZE);
    std::unordered_map<int, int> umap;
    
    for(int i = 1; i < 100; i++){
        lmap2.insert(i*i, i*i);
        umap[i*i] = i*i;
    }
    assert(is_equal(lmap2, umap));
    cout << "Put Test Pass!" << endl;
}

void test_duplicate_keys(){
    LinearMap2<int, int> lmap2(MEDIUM_SIZE);
    std::unordered_map<int, int> umap;

    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        lmap2.insert(i*i, i*i);
        umap[i*i] = i*i;
    }
    
    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        lmap2.insert(i*i, i);
        umap[i*i] = i;
    }
    
    assert(is_equal(lmap2, umap));
    cout << "Duplicate Key Test Pass!" << endl;
}

void test_reallocation(){
    LinearMap2<int, int> lmap2(SMALL_SIZE);
    std::unordered_map<int, int> umap;

    for(int i = 0; i < MEDIUM_SIZE; i++){
        lmap2.insert(i*i, i*i);
        umap[i*i] = i*i;
    }
    
    assert(is_equal(lmap2, umap));
    cout << "Reallocation Test Pass!" << endl;
}

void test_remove(){
    LinearMap2<int, int> lmap2(SMALL_SIZE);
    std::unordered_map<int, int> umap;
    std::vector<int> nums;
    
    for(int i = 0; i < MEDIUM_SIZE; i++){   
        lmap2.insert(i*i, i*i);
        umap[i*i] = i*i;
        nums.push_back(i*i);
    }

    for(int i = 0; i < MEDIUM_SIZE/2; i++){
        size_t idx = rand() % nums.size();
        int key = nums[idx];
        std::swap(nums[idx], nums[nums.size() - 1]);
        nums.pop_back();
        lmap2.remove(key);
        umap.erase(key);
    }

    for(int i = 0; i < MEDIUM_SIZE; i++){   
        int key = rand() % MEDIUM_SIZE/2;
        
        lmap2.insert(key, 2*key);
        umap[key] = 2*key;
    }

    assert(is_equal(lmap2, umap));

    cout << "Remove Test Pass!" << endl;
}

void test_interleaved1(){
    LinearMap2<int, int> lmap2(SMALL_SIZE);
    std::unordered_map<int, int> umap;
    size_t count = 0;
    for(int i = 0; i < LARGE_SIZE; i++){
        int action = rand() % 100;
        int key = rand() % LARGE_SIZE;
        int value = rand() % LARGE_SIZE;

        if(action < 65){
            lmap2.insert(key, value);
            umap[key] = value;    
        }
        else if(action < 90){
            lmap2.remove(key);
            umap.erase(key);
        }
        else{
            int candidate_value = -1;
            assert(lmap2.emplace(key, candidate_value) == umap.count(key));
            
            if(candidate_value != -1){
                assert(umap.at(key) == candidate_value);
            }
        }

        count++;

        if(count == 1000){
            assert(is_equal(lmap2, umap));
            count = 0;
        }
    }
    
    assert(is_equal(lmap2, umap));
    
    cout << "Interleaved Test 1 Pass!" << endl;
}

void test_interleaved2(){
    LinearMap2<int, int> lmap2(SMALL_SIZE);
    std::unordered_map<int, int> umap;
    size_t count = 0;
    for(int i = 0; i < LARGE_SIZE; i++){
        int action = rand() % 100;
        int key = rand() % 100;
        int value = rand() % 100;

        if(action < 65){
            lmap2.insert(key, value);
            umap[key] = value;    
        }
        else if(action < 90){
            lmap2.remove(key);
            umap.erase(key);
        }
        else{
            int candidate_value = -1;
            assert(lmap2.emplace(key, candidate_value) == umap.count(key));
            
            if(candidate_value != -1){
                assert(umap.at(key) == candidate_value);
            }
        }

        count++;

        if(count == 1000){
            assert(is_equal(lmap2, umap));
            count = 0;
        }
    }
    
    assert(is_equal(lmap2, umap));
    
    cout << "Interleaved Test 2 Pass!" << endl;
}

int main () {
    cout << "===========================" << endl;
    cout << "Starting LinearMap2 Tests" << endl;
    cout << "===========================" << endl;
    test_insert();
    test_duplicate_keys();
    test_reallocation();
    test_remove();
    test_interleaved1();
    test_interleaved2();
}