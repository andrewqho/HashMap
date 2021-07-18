#ifndef LINEAR_MAP_HPP 
#define LINEAR_MAP_HPP

#include <stdlib.h>
#include <stdio.h>

#include "entry.hpp"
#include "hashes.hpp"
using namespace std;

template <typename K, typename V, typename H = GenericHash<K> >
class LinearMap
{
    public:
        LinearMap(size_t init_capacity, double max_load=0.5) : hash_func(){   
            entries = new Entry<K, V>[init_capacity];
            num_entries = 0;
            capacity = init_capacity;
            load_factor = max_load;
        }
        // Destructor
        ~LinearMap(){
            delete entries;
        }
        LinearMap(const LinearMap& lmap){
            entries = new Entry<K, V>[lmap.getCapacity()];
            num_entries = 0;
            capacity = lmap.getCapacity();
            
            // insert all entries into new_entries
            for(auto i : lmap){
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

            // Hash the key and get the index
            size_t hash_val = hash_func(key, capacity);
            size_t idx = hash_val & (capacity-1);
            
            size_t avail_slot = -1;
            while(!entries[idx].isClean()){
                // Check if the key exists in the 
                // dictionary, if it does then replace it
                if(entries[idx].isOccupied() && entries[idx].key_cmp(key)){
                    entries[idx].populate(key, value, hash_val);
                    return;
                }
                else if(entries[idx].isDirty() && avail_slot == -1){
                    avail_slot = idx;
                }
                idx = (idx + 1) & (capacity-1);
            }
            
            if(avail_slot == -1){
                avail_slot = idx;
            }
            entries[avail_slot].populate(key, value, hash_val);
            num_entries++;
        }

        bool emplace(K const &key, V &value){
            size_t hash_val = hash_func(key, capacity);
            size_t idx = hash_val & (capacity-1);
            
            idx = find_index(key, idx);

            if(idx == -1){
                return false;
            }

            value = entries[idx].getValue();
            return true;
        }

        bool remove(K const &key){
            size_t hash_val = hash_func(key, capacity);
            size_t idx = hash_val & (capacity-1);
            
            idx = find_index(key, idx);

            if(idx == -1){
                return false;
            }

            entries[idx].clear();
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
        
        size_t find_index(K const &key, size_t idx){
            while(!entries[idx].isClean()){
                if(entries[idx].isOccupied() && entries[idx].key_cmp(key)){
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
                // Get the shit
                K key = old_entries[i].getKey();
                V value = old_entries[i].getValue();
                size_t hash_val = old_entries[i].getHash();
                size_t idx = hash_val & (new_capacity-1);
                
                // insert the key value pair into the new entry table
                while(!entries[idx].isClean()){
                    idx = (idx + 1) & (new_capacity-1);
                }
                entries[idx].populate(key, value, hash_val);
            }

            // Remove old entries
            delete old_entries;

            capacity = new_capacity;
        }
};

#endif