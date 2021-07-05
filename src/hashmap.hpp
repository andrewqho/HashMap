#include <stdlib.h>
#include <stdio.h>

#include "entry.hpp"
#include "hashes.hpp"
using namespace std;
// enum PROBE_TYPE = {LINEAR, QUADRATIC, ROBINHOOD}

// Init size should always be a power of 2
const double MAX_LOAD = 0.5;

template <typename K, typename V, typename H = GenericHash<K> >
class HashMap
{
    public:
        HashMap(size_t init_capacity){
            entries = new Entry<K, V> *[init_capacity];

            for(int i = 0; i < init_capacity; i++){
                entries[i] = NULL;
            }

            num_entries = 0;
            capacity = init_capacity;
        }
        ~HashMap(){
            for(int i = 0; i < capacity; i++){
                if(entries[i] == NULL){
                    continue;
                }
                delete entries[i];
            }
            delete entries;
        }
        void reallocate(){
            // Store old key value pairs
            Entry<K, V>** old_entries = entries;
            
            // Allocate a new array of Entry pointer objects
            entries = new Entry<K, V> *[2*capacity];

            for(int i = 0; i < 2*capacity; i++){
                entries[i] = NULL;
            }

            // Put all entries into new_entries
            for(int i = 0; i < capacity; i++){
                if(old_entries[i] == NULL){
                    continue;
                }
                // Get the shit
                K key = old_entries[i]->getKey();
                V value = old_entries[i]->getValue();

                size_t hash_val = old_entries[i]->getHash();
                size_t idx = hash_val & (2*capacity-1);
                
                // Put the key value pair into the new entry table
                while(entries[idx] != NULL && !entries[idx]->isEmpty()){
                    idx = (idx + 1) & (2*capacity-1);
                }

                entries[idx] = new Entry<K, V>(key, value, hash_val);

                // Delete entry object
                delete old_entries[i];
            }

            // Remove old entries
            delete old_entries;

            capacity *= 2;
        }
        
        void put(K const &key, V const &value){
            if(num_entries > MAX_LOAD*capacity){
                reallocate();
            }

            // Hash the key and get the index
            size_t hash_val = hash_func(key);
            size_t idx = hash_val & (capacity-1);
            if(find_index(key, idx) != -1){
                return;
            }
            
            while(entries[idx] != NULL && !entries[idx]->isEmpty()){
                if(entries[idx]->getKey() == key){
                    return;
                }
                idx = (idx + 1) & (capacity-1);
            }

            // Create a new entry object and put it into the index
            entries[idx] = new Entry<K, V>(key, value, hash_val);
            num_entries++;
        }

        size_t find_index(K const &key, size_t idx){
            while(entries[idx] != NULL){
                if(!entries[idx]->isEmpty() && key == entries[idx]->getKey()){
                    return idx;
                }
                idx = (idx + 1) & (capacity-1);
            }
            return -1;
        }

        bool get(K const &key, V &value){
            size_t hash_val = hash_func(key);
            size_t idx = hash_val & (capacity-1);
            idx = find_index(key, idx);
            if(idx != -1){
                value = entries[idx]->getValue();
                return true;
            }

            return false;
        }
        void remove(K const &key){
            size_t hash_val = hash_func(key);
            size_t idx = hash_val & (capacity-1);

            idx = find_index(key, idx);

            if(idx != -1){
                entries[idx]->setEmpty(true);
                num_entries--;
            }
        }

        size_t getSize(){
            return num_entries;
        }

        size_t getCapacity(){
            return capacity;
        }
    private: 
        Entry<K, V>** entries;
        size_t num_entries;
        size_t capacity;
        H hash_func;
};