#ifndef LINEAR_MAP2_HPP 
#define LINEAR_MAP2_HPP

#include <stdlib.h>
#include <stdio.h>

#include "entry2.hpp"
#include "hashes.hpp"
using namespace std;

template <typename K, typename V, typename H = GenericHash<K> >
class LinearMap2
{
    public:
        LinearMap2(size_t init_capacity=1024, double max_load=0.5) : hash_func(){   
            init_capacity = nextPowerOf2(init_capacity);
            entries = new Entry2<K, V>[init_capacity];
            num_entries = 0;
            capacity = init_capacity;
            load_factor = max_load;
        }
        // Destructor
        ~LinearMap2(){
            delete entries;
        }
        LinearMap2(const LinearMap2& lmap){
            entries = new Entry2<K, V>[lmap.getCapacity()];
            num_entries = 0;
            capacity = lmap.getCapacity();
            
            // insert all entries into new_entries
            for(auto i : lmap){
                K key = i.getKey();
                V value = i.getValue();
                size_t hash_val = i.getHash();
                size_t idx = hash_val & (capacity-1);

                setEntry(idx, key, value, hash_val);

                num_entries++;
            }
        }
        struct Iterator{
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = int;
            using value_type        = Entry2<K, V>;
            using pointer           = Entry2<K, V>*;  // or also value_type*
            using reference         = Entry2<K, V>&;  // or also value_type&

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
                    while(curr_idx < max_idx && !entry_ptr->isInUse()){
                        entry_ptr++;
                        curr_idx++;
                    }
                    return *this; 
                }
                Iterator operator++(int) { 
                    Iterator tmp = *this;
                    entry_ptr++;
                    curr_idx++;
                    while(curr_idx < max_idx && !entry_ptr->isInUse()){
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
        void printEntries(){
            for(int i = 0; i < capacity; i++){
                cout << i << " ";
                if(entries[i].isEmpty()){
                    cout << "NULL NULL" << endl;
                }
                else{
                    cout << entries[i].getKey() << " " << entries[i].getValue() << " " << entries[i].getPSL() << endl;
                }
            }
        }
        double calcAvgPSL(){
            double avg_PSL = 0.0;

            for(int i = 0; i < capacity; i++){
                if(entries[i].isInUse()){
                    avg_PSL += entries[i].getPSL();
                    
                }
            }

            return avg_PSL/num_entries; 
        }

        size_t calcMaxPSL(){
            size_t max_PSL = 0;

            for(int i = 0; i < capacity; i++){
                if(entries[i].isInUse()){
                    max_PSL = max(max_PSL, entries[i].getPSL());
                    
                }
            }

            return max_PSL; 
        }

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
                if(entries[idx].isInUse() && entries[idx].key_cmp(key)){
                    setEntry(idx, key, value, hash_val);
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
            setEntry(idx, key, value, hash_val);
            
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
            while(curr_idx < capacity && !entries[curr_idx].isInUse()){
                curr_idx++;
            }
            return Iterator(&entries[curr_idx], curr_idx, capacity); 
        }

        Iterator end() {
            return Iterator(&entries[capacity], capacity, capacity);
        }
    private: 
        Entry2<K, V>* entries;
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

        size_t convertHash(size_t hash_val){
            return hash_val & (capacity - 1);
        }
        
        size_t nextIdx(size_t idx){
            return (idx+1) & (capacity - 1);
        }

        size_t find_index(K const &key, size_t idx){
            while(!entries[idx].isClean()){
                if(entries[idx].isInUse() && entries[idx].key_cmp(key)){
                    return idx;
                }
                idx = (idx + 1) & (capacity-1);
            }
            return -1;
        }
        void setEntry(size_t const idx, K const &key, V const &value, size_t const hash_val){
            size_t PSL = calcPSL(hash_val, idx);

            entries[idx].setKey(key);
            entries[idx].setValue(value);
            entries[idx].setHash(hash_val);
            entries[idx].setPSL(PSL);

            entries[idx].setState(IN_USE);
       
        }

        size_t calcPSL(size_t hash_val, size_t actual_idx){
            size_t expected_idx = convertHash(hash_val);
            return calcDist(expected_idx, actual_idx);
        }

        size_t calcDist(size_t start_idx, size_t end_idx){
            size_t dist = end_idx - start_idx;
            if(dist < 0){
                dist += capacity;
            }
            return dist;
        }

        void reallocate(){
            // Calculate new capacity
            size_t new_capacity = 2*capacity;

            // Store old key value pairs
            Entry2<K, V>* old_entries = entries;
            
            // Allocate a new array of Entry pointer objects
            entries = new Entry2<K, V>[new_capacity];

            // insert all entries into new_entries
            for(int i = 0; i < capacity; i++){
                if(!old_entries[i].isInUse()){
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
                setEntry(idx, key, value, hash_val);
            }

            // Remove old entries
            delete old_entries;

            capacity = new_capacity;
        }
};

#endif