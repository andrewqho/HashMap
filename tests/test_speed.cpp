#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <math.h>   
#include <unordered_map>
#include "linear_map.hpp"
#include "linear_map2.hpp"
#include "robinhood_map.hpp"
using namespace std;

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

const uint64_t SMALL_SIZE = pow(2, 4);
const uint64_t MEDIUM_SIZE = pow(2, 15);
const uint64_t LARGE_SIZE = pow(2, 20);

const uint64_t MAX_ELEM = 100000;
const uint64_t STEP_SIZE = MAX_ELEM/20;

double umap_insert_speed(std::unordered_map<uint64_t, uint64_t> &umap, std::vector<uint64_t> keys){
    auto t1 = steady_clock::now();
    for(uint64_t key : keys){
        umap[key] = 1;    
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double umap_emplace_speed(std::unordered_map<uint64_t, uint64_t> &umap, std::vector<uint64_t> keys){
    uint64_t value;
    uint64_t sum = 0;
    auto t1 = steady_clock::now();
    for(uint64_t key: keys){
        umap.emplace(key, value);
        sum += value;
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double umap_remove_speed(std::unordered_map<uint64_t, uint64_t> &umap, std::vector<uint64_t> keys){
    auto t1 = steady_clock::now();
    for(uint64_t key: keys){
        umap.erase(key);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap_insert_speed(LinearMap<uint64_t, uint64_t> &lmap, std::vector<uint64_t> keys){
    auto t1 = steady_clock::now();
    for(uint64_t key : keys){
        lmap.insert(key, 1);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap_emplace_speed(LinearMap<uint64_t, uint64_t> &lmap, std::vector<uint64_t> keys){
    uint64_t value;
    uint64_t sum = 0;
    auto t1 = steady_clock::now();
    for(uint64_t key : keys){
        lmap.emplace(key, value);
        sum += value;
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap_remove_speed(LinearMap<uint64_t, uint64_t> &lmap, std::vector<uint64_t> keys){
    auto t1 = steady_clock::now();
    for(uint64_t key : keys){
        lmap.remove(key);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double rmap_insert_speed(RobinhoodMap<uint64_t, uint64_t> &rmap, std::vector<uint64_t> keys){
    auto t1 = steady_clock::now();
    for(uint64_t key : keys){
        rmap.insert(key, 1);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double rmap_emplace_speed(RobinhoodMap<uint64_t, uint64_t> &rmap, std::vector<uint64_t> keys){
    uint64_t value;
    uint64_t sum = 0;
    auto t1 = steady_clock::now();
    for(uint64_t key : keys){
        rmap.emplace(key, value);
        sum += value;
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double rmap_remove_speed(RobinhoodMap<uint64_t, uint64_t> &rmap, std::vector<uint64_t> keys){
    auto t1 = steady_clock::now();
    for(uint64_t key : keys){
        rmap.remove(key);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

void test_insert_speed(){
    std::ofstream benchmark_insert;
    benchmark_insert.open("bin/benchmarks/insert.csv");

    benchmark_insert << "Num Entries,Linear Map,Robinhood Map,Unordered Map\n";

    // Declare hashmaps
    LinearMap<uint64_t, uint64_t> lmap(SMALL_SIZE);
    RobinhoodMap<uint64_t, uint64_t> rmap(SMALL_SIZE);
    std::unordered_map<uint64_t, uint64_t> umap;

    for(uint64_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Generate keys
        std::vector<uint64_t> keys;
        for(int j = 0; j < i; j++){
            keys.push_back(j);
        }
        std::random_shuffle (keys.begin(), keys.end());

        benchmark_insert << i << ",";
        benchmark_insert << lmap_insert_speed(lmap, keys) << ",";;
        benchmark_insert << rmap_insert_speed(rmap, keys) << ",";;
        benchmark_insert << umap_insert_speed(umap, keys) << "\n";
    }    
    cout << "Finished Simple Insertion Speed Test" << endl;
}

void test_emplace_speed(){
    std::ofstream benchmark_emplace;
    benchmark_emplace.open("bin/benchmarks/emplace.csv");

    benchmark_emplace << "Num Entries,Linear Map,Robinhood Map,Unordered Map\n";

    // Declare hashmaps
    LinearMap<uint64_t, uint64_t> lmap(SMALL_SIZE);
    RobinhoodMap<uint64_t, uint64_t> rmap(SMALL_SIZE);
    std::unordered_map<uint64_t, uint64_t> umap;

    for(uint64_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Generate keys
        std::vector<uint64_t> keys;
        for(int j = 0; j < i; j++){
            keys.push_back(j);
        }
        std::random_shuffle (keys.begin(), keys.end());
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::random_shuffle (keys.begin(), keys.end());
        
        benchmark_emplace << i << ",";
        benchmark_emplace << lmap_emplace_speed(lmap, keys) << ",";
        benchmark_emplace << rmap_emplace_speed(rmap, keys) << ",";
        benchmark_emplace << umap_emplace_speed(umap, keys) << "\n";
    }
    cout << "Finished Simple Emplace Speed Test" << endl;
}

void test_remove_speed(){
    std::ofstream benchmark_remove;
    benchmark_remove.open("bin/benchmarks/remove.csv");

    benchmark_remove << "Num Entries,Linear Map,Robinhood Map,Unordered Map\n";

    // Declare hashmaps
    LinearMap<uint64_t, uint64_t> lmap(SMALL_SIZE);
    RobinhoodMap<uint64_t, uint64_t> rmap(SMALL_SIZE);
    std::unordered_map<uint64_t, uint64_t> umap;

    for(uint64_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Generate keys
        std::vector<uint64_t> keys;
        for(int j = 0; j < i; j++){
            keys.push_back(j);
        }
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::random_shuffle (keys.begin(), keys.end());
        
        benchmark_remove << i << ",";
        benchmark_remove << lmap_remove_speed(lmap, keys) << ",";
        benchmark_remove << rmap_remove_speed(rmap, keys) << ",";
        benchmark_remove << umap_remove_speed(umap, keys) << "\n";
    }    
    cout << "Finished Simple Remove Speed Test" << endl;
}

void test_insert_with_removal_speed(){
    std::ofstream benchmark_insert_with_removal;
    benchmark_insert_with_removal.open("bin/benchmarks/insert_with_removal.csv");

    benchmark_insert_with_removal << "Num Entries,Linear Map,Robinhood Map,Unordered Map\n";

    // Declare hashmaps
    LinearMap<uint64_t, uint64_t> lmap(SMALL_SIZE);
    RobinhoodMap<uint64_t, uint64_t> rmap(SMALL_SIZE);
    std::unordered_map<uint64_t, uint64_t> umap;

    for(uint64_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Generate keys
        std::vector<uint64_t> keys;
        for(int j = 0; j < i; j++){
            keys.push_back(j);
        }
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::vector<uint64_t> keys_to_remove = std::vector<uint64_t>(keys.begin(), keys.end() - i/2);

        std::random_shuffle (keys_to_remove.begin(), keys_to_remove.end());
        
        lmap_remove_speed(lmap, keys_to_remove);
        rmap_remove_speed(rmap, keys_to_remove);
        umap_remove_speed(umap, keys_to_remove);

        std::random_shuffle (keys.begin(), keys.end());
        
        benchmark_insert_with_removal << i << ",";
        benchmark_insert_with_removal << lmap_insert_speed(lmap, keys) << ",";
        benchmark_insert_with_removal << rmap_insert_speed(rmap, keys) << ",";
        benchmark_insert_with_removal << umap_insert_speed(umap, keys) << "\n";
    }    
    cout << "Finished Insert with Removals Speed Test" << endl;
}

void test_emplace_with_removal_speed(){
    std::ofstream benchmark_emplace_with_removal;
    benchmark_emplace_with_removal.open("bin/benchmarks/emplace_with_removal.csv");

    benchmark_emplace_with_removal << "Num Entries,Linear Map,Robinhood Map,Unordered Map\n";

    // Declare hashmaps
    LinearMap<uint64_t, uint64_t> lmap(SMALL_SIZE);
    RobinhoodMap<uint64_t, uint64_t> rmap(SMALL_SIZE);
    std::unordered_map<uint64_t, uint64_t> umap;

    for(uint64_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Generate keys
        std::vector<uint64_t> keys;
        for(int j = 0; j < i; j++){
            keys.push_back(j);
        }
        std::random_shuffle (keys.begin(), keys.end());
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::vector<uint64_t> keys_to_remove = std::vector<uint64_t>(keys.begin(), keys.end() - i/2);

        std::random_shuffle (keys_to_remove.begin(), keys_to_remove.end());
        
        lmap_remove_speed(lmap, keys_to_remove);
        rmap_remove_speed(rmap, keys_to_remove);
        umap_remove_speed(umap, keys_to_remove);

        std::random_shuffle (keys.begin(), keys.end());
        
        benchmark_emplace_with_removal << i << ",";
        benchmark_emplace_with_removal << lmap_emplace_speed(lmap, keys) << ",";
        benchmark_emplace_with_removal << rmap_emplace_speed(rmap, keys) << ",";
        benchmark_emplace_with_removal << umap_emplace_speed(umap, keys) << "\n";
    }
    cout << "Finished Emplace with Removals Speed Test" << endl;    
}

int main () {
    cout << "===========================" << endl;
    cout << "Starting Speed Tests" << endl;
    cout << "===========================" << endl;

    test_insert_speed();
    test_emplace_speed();
    test_remove_speed();
    test_insert_with_removal_speed();
    test_emplace_with_removal_speed();
}