#ifndef ROBINHOOD_MAP_HPP 
#define ROBINHOOD_MAP_HPP

#include <stdlib.h>
#include <stdio.h>

#include "entry.hpp"
#include "hashes.hpp"
using namespace std;

template <typename K, typename V, typename H = GenericHash<K> >
class RobinhoodMap
{
    public:
        RobinhoodMap(size_t init_capacity=1024, double max_load=0.5) : hash_func(){   
            init_capacity = nextPowerOf2(init_capacity);
            entries = new Entry<K, V>[init_capacity];
            num_entries = 0;
            capacity = init_capacity;
            load_factor = max_load;
        }
        // Destructor
        ~RobinhoodMap(){
            delete entries;
        }
        RobinhoodMap(const RobinhoodMap& rmap){
            entries = new Entry<K, V>[rmap.getCapacity()];
            num_entries = 0;
            capacity = rmap.getCapacity();
            load_factor = rmap.getLoadFactor();
            // insert all entries into new_entries
            for(auto i : rmap){
                K key = i.getKey();
                V value = i.getValue();
                size_t hash_val = i.getHash();
                size_t idx = hash_val & (capacity-1);

                entries[idx].populate(key, value, hash_val);

                num_entries++;
            }
        }
        struct Iterator{
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = int;
            using value_type        = Entry<K, V>;
            using pointer           = Entry<K, V>*;  // or also value_type*
            using reference         = Entry<K, V>&;  // or also value_type&

            public:
                Iterator(pointer ptr, size_t idx, size_t capacity): entry_ptr(ptr), curr_idx(idx), max_idx(capacity) {}
                reference operator*() const {
                    return *entry_ptr; 
                }
                pointer operator->() { 
                    return entry_ptr; 
                }
                Iterator operator++() { 
                    entry_ptr++;
                    curr_idx++;
                    while(curr_idx < max_idx && !entry_ptr->isOccupied()){
                        entry_ptr++;
                        curr_idx++;
                    }
                    return *this; 
                }
                Iterator operator++(int) { 
                    Iterator tmp = *this;
                    entry_ptr++;
                    curr_idx++;
                    while(curr_idx < max_idx && !entry_ptr->isOccupied()){
                        entry_ptr++;
                        curr_idx++;
                    }
                    return tmp;
                    
                }
                friend bool operator== (const Iterator& a, const Iterator& b) { 
                    return a.entry_ptr == b.entry_ptr; 
                };
                friend bool operator!= (const Iterator& a, const Iterator& b) { 
                    return a.entry_ptr != b.entry_ptr; 
                };  
            private:
                pointer entry_ptr;
                size_t curr_idx;
                size_t max_idx;
        };

        void insert(K const &key, V const &value){
            if(num_entries > load_factor*capacity){
                reallocate();
            }

            K key_to_write = key;
            V val_to_write = value;
            // Hash the key and get the index
            size_t hash_val = hash_func(key, capacity);
            size_t idx = hash_val & (capacity-1);
            int PSL = 0;

            while(!entries[idx].isEmpty()){
                // Check if the key exists in the 
                // dictionary, if it does then replace it
                if(entries[idx].isOccupied() && entries[idx].key_cmp(key)){
                    entries[idx].populate(key, value, hash_val);
                    return;
                }
                // Check if we should swap
                if(entries[idx].getPSL() < PSL){
                    K temp_key = entries[idx].getKey();
                    V temp_val = entries[idx].getValue();
                    size_t temp_hash = entries[idx].getHash();
                    size_t temp_PSL = entries[idx].getPSL();

                    entries[idx].populate(key_to_write, val_to_write, hash_val);
                    entries[idx].setPSL(PSL);

                    key_to_write = temp_key;
                    val_to_write = temp_val;
                    hash_val = temp_hash;
                    PSL = temp_PSL;
                }
                idx = (idx + 1) & (capacity-1);
                PSL++;
            }
            
            entries[idx].populate(key_to_write, val_to_write, hash_val);
            entries[idx].setPSL(PSL);
            num_entries++;
        }

        bool emplace(K const &key, V &value){            
            int idx = find_index(key);

            if(idx == -1){
                return false;
            }

            value = entries[idx].getValue();
            return true;
        }

        bool remove(K const &key){
            size_t idx = find_index(key);

            if(idx == -1){
                return false;
            }
            
            // Perform backwards shifting
            entries[idx].setState(EMPTY);
            
            size_t next_idx = (idx+1) & (capacity - 1);
            while(entries[next_idx].isOccupied() && entries[next_idx].getPSL() > 0){
                K next_key = entries[next_idx].getKey();
                V next_val = entries[next_idx].getValue();
                size_t next_hash = entries[next_idx].getHash();
                size_t next_PSL = entries[next_idx].getPSL();
                entries[idx].populate(next_key, next_val, next_hash);
                entries[idx].setPSL(next_PSL - 1);
                idx = next_idx;
                entries[idx].setState(EMPTY);
                next_idx = (idx+1) & (capacity - 1);
            }

            num_entries--;

            return true;
        }

        size_t getSize(){
            return num_entries;
        }

        size_t getCapacity(){
            return capacity;
        }

        Iterator begin() { 
            size_t curr_idx = 0;
            while(curr_idx < capacity && !entries[curr_idx].isOccupied()){
                curr_idx++;
            }
            return Iterator(&entries[curr_idx], curr_idx, capacity); 
        }

        Iterator end() {
            return Iterator(&entries[capacity], capacity, capacity);
        }
    private: 
        Entry<K, V>* entries;
        size_t num_entries;
        size_t capacity;
        H hash_func;
        double load_factor;
        
        unsigned int nextPowerOf2(unsigned int n){
            n--;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            n++;
            return n;
        }

        size_t find_index(K const &key){
            size_t hash_val = hash_func(key, capacity);
            int idx = hash_val & (capacity-1);
            
            while(entries[idx].isOccupied()){
                if(entries[idx].key_cmp(key)){
                    return idx;
                }
                idx = (idx + 1) & (capacity-1);
            }
            return -1;
        }
        
        void reallocate(){
            // Calculate new capacity
            size_t new_capacity = 2*capacity;

            // Store old key value pairs
            Entry<K, V>* old_entries = entries;
            
            // Allocate a new array of Entry pointer objects
            entries = new Entry<K, V>[new_capacity];

            // insert all entries into new_entries
            for(int i = 0; i < capacity; i++){
                if(!old_entries[i].isOccupied()){
                    continue;
                }
                K key_to_write = old_entries[i].getKey();
                V val_to_write = old_entries[i].getValue();

                // Hash the key and get the index
                size_t hash_val = old_entries[i].getHash();
                size_t idx = hash_val & (new_capacity-1);
                int PSL = 0;

                while(!entries[idx].isEmpty()){
                    // Check if we should swap
                    if(entries[idx].getPSL() < PSL){
                        K temp_key = entries[idx].getKey();
                        V temp_val = entries[idx].getValue();
                        size_t temp_hash = entries[idx].getHash();
                        size_t temp_PSL = entries[idx].getPSL();

                        entries[idx].populate(key_to_write, val_to_write, hash_val);
                        entries[idx].setPSL(PSL);

                        key_to_write = temp_key;
                        val_to_write = temp_val;
                        hash_val = temp_hash;
                        PSL = temp_PSL;
                    }
                    idx = (idx + 1) & (new_capacity-1);
                    PSL++;
                }        
                entries[idx].populate(key_to_write, val_to_write, hash_val);
                entries[idx].setPSL(PSL);
            }

            // Remove old entries
            delete old_entries;

            capacity = new_capacity;
        }
};

#endif