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

const uint32_t SMALL_SIZE = pow(2, 10);
const uint32_t MEDIUM_SIZE = pow(2, 15);
const uint32_t LARGE_SIZE = pow(2, 20);

const uint32_t MAX_ELEM = 100000;
const uint32_t STEP_SIZE = MAX_ELEM/20;

double umap_insert_speed(std::unordered_map<uint32_t, uint32_t> &umap, std::vector<uint32_t> keys){
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        umap[key] = 1;    
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double umap_emplace_speed(std::unordered_map<uint32_t, uint32_t> &umap, std::vector<uint32_t> keys){
    uint32_t value;
    uint32_t sum = 0;
    auto t1 = steady_clock::now();
    for(uint32_t key: keys){
        umap.emplace(key, value);
        sum += value;
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double umap_remove_speed(std::unordered_map<uint32_t, uint32_t> &umap, std::vector<uint32_t> keys){
    auto t1 = steady_clock::now();
    for(uint32_t key: keys){
        umap.erase(key);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap_insert_speed(LinearMap<uint32_t, uint32_t> &lmap, std::vector<uint32_t> keys){
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        lmap.insert(key, 1);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap_emplace_speed(LinearMap<uint32_t, uint32_t> &lmap, std::vector<uint32_t> keys){
    uint32_t value;
    uint32_t sum = 0;
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        lmap.emplace(key, value);
        sum += value;
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap_remove_speed(LinearMap<uint32_t, uint32_t> &lmap, std::vector<uint32_t> keys){
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        lmap.remove(key);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap2_insert_speed(LinearMap2<uint32_t, uint32_t> &lmap2, std::vector<uint32_t> keys){
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        lmap2.insert(key, 1);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap2_emplace_speed(LinearMap2<uint32_t, uint32_t> &lmap2, std::vector<uint32_t> keys){
    uint32_t value;
    uint32_t sum = 0;
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        lmap2.emplace(key, value);
        sum += value;
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double lmap2_remove_speed(LinearMap2<uint32_t, uint32_t> &lmap2, std::vector<uint32_t> keys){
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        lmap2.remove(key);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double rmap_insert_speed(RobinhoodMap<uint32_t, uint32_t> &rmap, std::vector<uint32_t> keys){
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        rmap.insert(key, 1);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double rmap_emplace_speed(RobinhoodMap<uint32_t, uint32_t> &rmap, std::vector<uint32_t> keys){
    uint32_t value;
    uint32_t sum = 0;
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        rmap.emplace(key, value);
        sum += value;
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

double rmap_remove_speed(RobinhoodMap<uint32_t, uint32_t> &rmap, std::vector<uint32_t> keys){
    auto t1 = steady_clock::now();
    for(uint32_t key : keys){
        rmap.remove(key);
    }
    auto t2 = steady_clock::now();
    duration<double> s_double = t2 - t1;
    return s_double.count();
}

void benchmark_insert_speed(){
    std::ofstream benchmark_insert;
    benchmark_insert.open("bin/benchmarks/insert.csv");

    benchmark_insert << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map,Unordered Map\n";

    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Declare hashmaps
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
        std::unordered_map<uint32_t, uint32_t> umap;

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < i; j++){
            keys.push_back(rand());
        }
        std::random_shuffle (keys.begin(), keys.end());

        benchmark_insert << i << ",";
        benchmark_insert << lmap2_insert_speed(lmap2, keys) << ",";
        benchmark_insert << lmap_insert_speed(lmap, keys) << ",";
        benchmark_insert << rmap_insert_speed(rmap, keys) << ",";
        benchmark_insert << umap_insert_speed(umap, keys) << "\n";
    }    
    cout << "Finished Simple Insertion Speed Benchmark" << endl;
}

void benchmark_emplace_speed(){
    std::ofstream benchmark_emplace;
    benchmark_emplace.open("bin/benchmarks/emplace.csv");

    benchmark_emplace << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map,Unordered Map\n";

    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Declare hashmaps
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
        std::unordered_map<uint32_t, uint32_t> umap;

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < i; j++){
            keys.push_back(rand());
        }
        std::random_shuffle (keys.begin(), keys.end());
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::random_shuffle (keys.begin(), keys.end());
        
        benchmark_emplace << i << ",";
        benchmark_emplace << lmap2_emplace_speed(lmap2, keys) << ",";
        benchmark_emplace << lmap_emplace_speed(lmap, keys) << ",";
        benchmark_emplace << rmap_emplace_speed(rmap, keys) << ",";
        benchmark_emplace << umap_emplace_speed(umap, keys) << "\n";
    }
    cout << "Finished Simple Successful Emplace Speed Benchmark" << endl;
}

void benchmark_unsuccessful_emplace_speed(){
    std::ofstream benchmark_emplace;
    benchmark_emplace.open("bin/benchmarks/unsuccessful_emplace.csv");

    benchmark_emplace << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map,Unordered Map\n";

    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Declare hashmaps
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
        std::unordered_map<uint32_t, uint32_t> umap;

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < i; j++){
            keys.push_back(rand());
        }
        std::random_shuffle (keys.begin(), keys.end());
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::random_shuffle (keys.begin(), keys.end());
        
        std::vector<uint32_t> new_keys;
        for(uint32_t j = 0; j < i; j++){
            new_keys.push_back(rand());
        }
        benchmark_emplace << i << ",";
        benchmark_emplace << lmap2_emplace_speed(lmap2, new_keys) << ",";
        benchmark_emplace << lmap_emplace_speed(lmap, new_keys) << ",";
        benchmark_emplace << rmap_emplace_speed(rmap, new_keys) << ",";
        benchmark_emplace << umap_emplace_speed(umap, new_keys) << "\n";
    }
    cout << "Finished Simple Unsuccessful Emplace Speed Benchmark" << endl;
}

void benchmark_remove_speed(){
    std::ofstream benchmark_remove;
    benchmark_remove.open("bin/benchmarks/remove.csv");

    benchmark_remove << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map,Unordered Map\n";

    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Declare hashmaps
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
        std::unordered_map<uint32_t, uint32_t> umap;

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < i; j++){
            keys.push_back(rand());
        }
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::random_shuffle (keys.begin(), keys.end());
        
        benchmark_remove << i << ",";
        benchmark_remove << lmap2_remove_speed(lmap2, keys) << ",";
        benchmark_remove << lmap_remove_speed(lmap, keys) << ",";
        benchmark_remove << rmap_remove_speed(rmap, keys) << ",";
        benchmark_remove << umap_remove_speed(umap, keys) << "\n";
    }    
    cout << "Finished Simple Remove Speed Benchmark" << endl;
}

void benchmark_insert_with_removal_speed(){
    std::ofstream benchmark_insert_with_removal;
    benchmark_insert_with_removal.open("bin/benchmarks/insert_with_removal.csv");

    benchmark_insert_with_removal << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map,Unordered Map\n";

    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Declare hashmaps
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
        std::unordered_map<uint32_t, uint32_t> umap;

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < i; j++){
            keys.push_back(rand());
        }
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::vector<uint32_t> keys_to_remove = std::vector<uint32_t>(keys.begin(), keys.end() - i/2);

        std::random_shuffle (keys_to_remove.begin(), keys_to_remove.end());
        
        lmap2_remove_speed(lmap2, keys_to_remove);
        lmap_remove_speed(lmap, keys_to_remove);
        rmap_remove_speed(rmap, keys_to_remove);
        umap_remove_speed(umap, keys_to_remove);

        std::random_shuffle (keys.begin(), keys.end());
        
        benchmark_insert_with_removal << i << ",";
        benchmark_insert_with_removal << lmap2_insert_speed(lmap2, keys) << ",";
        benchmark_insert_with_removal << lmap_insert_speed(lmap, keys) << ",";
        benchmark_insert_with_removal << rmap_insert_speed(rmap, keys) << ",";
        benchmark_insert_with_removal << umap_insert_speed(umap, keys) << "\n";
    }    
    cout << "Finished Insert with Removals Speed Benchmark" << endl;
}

void benchmark_emplace_with_removal_speed(){
    std::ofstream benchmark_emplace_with_removal;
    benchmark_emplace_with_removal.open("bin/benchmarks/emplace_with_removal.csv");

    benchmark_emplace_with_removal << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map,Unordered Map\n";

    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Declare hashmaps
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
        std::unordered_map<uint32_t, uint32_t> umap;

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < i; j++){
            keys.push_back(rand());
        }
        std::random_shuffle (keys.begin(), keys.end());
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::vector<uint32_t> keys_to_remove = std::vector<uint32_t>(keys.begin(), keys.end() - i/2);

        std::random_shuffle (keys_to_remove.begin(), keys_to_remove.end());
        
        lmap2_remove_speed(lmap2, keys_to_remove);
        lmap_remove_speed(lmap, keys_to_remove);
        rmap_remove_speed(rmap, keys_to_remove);
        umap_remove_speed(umap, keys_to_remove);

        std::random_shuffle (keys.begin(), keys.end());
        
        benchmark_emplace_with_removal << i << ",";
        benchmark_emplace_with_removal << lmap2_emplace_speed(lmap2, keys) << ",";
        benchmark_emplace_with_removal << lmap_emplace_speed(lmap, keys) << ",";
        benchmark_emplace_with_removal << rmap_emplace_speed(rmap, keys) << ",";
        benchmark_emplace_with_removal << umap_emplace_speed(umap, keys) << "\n";
    }
    cout << "Finished Emplace with Removals Speed Benchmark" << endl;    
}

void benchmark_unsuccessful_emplace_with_removal_speed(){
    std::ofstream benchmark_emplace_with_removal;
    benchmark_emplace_with_removal.open("bin/benchmarks/unsuccessful_emplace_with_removal.csv");

    benchmark_emplace_with_removal << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map,Unordered Map\n";

    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Declare hashmaps
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
        std::unordered_map<uint32_t, uint32_t> umap;

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < i; j++){
            keys.push_back(rand());
        }
        std::random_shuffle (keys.begin(), keys.end());
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);
        
        std::vector<uint32_t> keys_to_remove = std::vector<uint32_t>(keys.begin(), keys.end() - i/2);

        std::random_shuffle (keys_to_remove.begin(), keys_to_remove.end());
        
        lmap2_remove_speed(lmap2, keys_to_remove);
        lmap_remove_speed(lmap, keys_to_remove);
        rmap_remove_speed(rmap, keys_to_remove);
        umap_remove_speed(umap, keys_to_remove);

        
        std::vector<uint32_t> new_keys;
        for(uint32_t j = 0; j < i; j++){
            new_keys.push_back(rand());
        }

        std::random_shuffle (new_keys.begin(), new_keys.end());
        
        benchmark_emplace_with_removal << i << ",";
        benchmark_emplace_with_removal << lmap2_emplace_speed(lmap2, new_keys) << ",";
        benchmark_emplace_with_removal << lmap_emplace_speed(lmap, new_keys) << ",";
        benchmark_emplace_with_removal << rmap_emplace_speed(rmap, new_keys) << ",";
        benchmark_emplace_with_removal << umap_emplace_speed(umap, new_keys) << "\n";
    }
    cout << "Finished Unsuccessful Emplace with Removals Speed Benchmark" << endl;    
}

void benchmark_avg_PSL(){
    std::ofstream benchmark_PSL;
    benchmark_PSL.open("bin/benchmarks/avg_PSL.csv");

    benchmark_PSL << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map\n";

    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        // Declare hashmaps
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE, 0.95);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE, 0.95);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE, 0.95);

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < i; j++){
            keys.push_back(rand());
        }
        
        std::random_shuffle (keys.begin(), keys.end());
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);

        benchmark_PSL << i << ",";
        benchmark_PSL << lmap2.calcAvgPSL() << ",";
        benchmark_PSL << lmap.calcAvgPSL() << ",";
        benchmark_PSL << rmap.calcAvgPSL() << endl;
    }
    cout << "Finished Average PSL Benchmark" << endl;
}

void benchmark_max_PSL(){
    std::ofstream benchmark_PSL;
    benchmark_PSL.open("bin/benchmarks/max_PSL.csv");

    benchmark_PSL << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map\n";

    // Declare hashmaps
    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
            
        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < MAX_ELEM; j++){
            keys.push_back(rand());
        }
        
        std::random_shuffle (keys.begin(), keys.end());
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);

        benchmark_PSL << i << ",";
        benchmark_PSL << lmap2.calcMaxPSL() << ",";
        benchmark_PSL << lmap.calcMaxPSL() << ",";
        benchmark_PSL << rmap.calcMaxPSL() << endl;

        
    }
    cout << "Finished Max PSL Benchmark" << endl;
}

void benchmark_memory_usage(){
    std::ofstream benchmark_memory;
    benchmark_memory.open("bin/benchmarks/memory.csv");

    benchmark_memory << "Num Entries,LinearMap with Tombstone,LinearMap with Backwards Shifting,Robinhood Map\n";

    // Declare hashmaps
    for(uint32_t i = STEP_SIZE; i <= MAX_ELEM; i+=STEP_SIZE){
        LinearMap2<uint32_t, uint32_t> lmap2(SMALL_SIZE);
        LinearMap<uint32_t, uint32_t> lmap(SMALL_SIZE);
        RobinhoodMap<uint32_t, uint32_t> rmap(SMALL_SIZE);
        std::unordered_map<uint32_t, uint32_t> umap(SMALL_SIZE);

        // Generate keys
        std::vector<uint32_t> keys;
        for(uint32_t j = 0; j < MAX_ELEM; j++){
            keys.push_back(rand());
        }
        
        std::random_shuffle (keys.begin(), keys.end());
        lmap2_insert_speed(lmap2, keys);
        lmap_insert_speed(lmap, keys);
        rmap_insert_speed(rmap, keys);
        umap_insert_speed(umap, keys);

        benchmark_memory << i << ",";
        benchmark_memory << sizeof(lmap2) << ",";
        benchmark_memory << sizeof(lmap) << ",";
        benchmark_memory << sizeof(rmap) << ",";
        benchmark_memory << sizeof(umap) << endl;
    }
    cout << "Finished Memory Usage Benchmark" << endl;
}

int main () {
    srand(time(0));

    cout << "===============================" << endl;
    cout << "Starting Performance Benchmarks" << endl;
    cout << "===============================" << endl;

    benchmark_insert_speed();
    benchmark_emplace_speed();
    benchmark_unsuccessful_emplace_speed();
    benchmark_remove_speed();
    benchmark_insert_with_removal_speed();
    benchmark_emplace_with_removal_speed();
    benchmark_unsuccessful_emplace_with_removal_speed();
    // benchmark_memory_usage();
    // benchmark_max_PSL();
    // benchmark_avg_PSL();
}