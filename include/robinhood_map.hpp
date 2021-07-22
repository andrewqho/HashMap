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
        RobinhoodMap(size_t init_capacity=1024, double max_load=0.5) : hash_func() 
        {   
            init_capacity = nextPowerOf2(init_capacity);
            entries = new Entry<K, V>[init_capacity];
            num_entries = 0;
            capacity = init_capacity;
            load_factor = max_load;
        }
        // Destructor
        ~RobinhoodMap()
        {
            delete entries;
        }
        RobinhoodMap(const RobinhoodMap& rmap)
        {
            entries = new Entry<K, V>[rmap.getCapacity()];
            num_entries = rmap.getSize();
            capacity = rmap.getCapacity();
            load_factor = rmap.getLoadFactor();
            // insert all entries into new_entries
            for(auto i : rmap)
            {
                K key = i.getKey();
                V value = i.getValue();
                size_t hash_val = i.getHash();
                size_t idx = convertHash(idx);

                setEntry(idx, key, value, hash_val);
            }
        }
        struct Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = int;
            using value_type        = Entry<K, V>;
            using pointer           = Entry<K, V>*;  // or also value_type*
            using reference         = Entry<K, V>&;  // or also value_type&

            public:
                Iterator(pointer ptr, size_t idx, size_t capacity): entry_ptr(ptr), curr_idx(idx), max_idx(capacity) {}
                reference operator*() const 
                {
                    return *entry_ptr; 
                }
                pointer operator->() 
                { 
                    return entry_ptr; 
                }
                Iterator operator++() 
                { 
                    entry_ptr++;
                    curr_idx++;
                    while(curr_idx < max_idx && !entry_ptr->isOccupied()){
                        entry_ptr++;
                        curr_idx++;
                    }
                    return *this; 
                }
                Iterator operator++(int) 
                { 
                    Iterator tmp = *this;
                    entry_ptr++;
                    curr_idx++;
                    while(curr_idx < max_idx && !entry_ptr->isOccupied())
                    {
                        entry_ptr++;
                        curr_idx++;
                    }
                    return tmp;
                    
                }
                friend bool operator== (const Iterator& a, const Iterator& b) 
                { 
                    return a.entry_ptr == b.entry_ptr; 
                };
                friend bool operator!= (const Iterator& a, const Iterator& b) 
                { 
                    return a.entry_ptr != b.entry_ptr; 
                };  
            private:
                pointer entry_ptr;
                size_t curr_idx;
                size_t max_idx;
        };

        void insert(K const &key, V const &value)
        {
            if(num_entries > load_factor*capacity)
            {
                reallocate();
            }

            K key_to_write = key;
            V val_to_write = value;
            // Hash the key and get the index
            size_t hash_val = hash_func(key, capacity);
            size_t idx = convertHash(hash_val);
            size_t PSL = 0;

            while(!entries[idx].isEmpty())
            {
                // Check if the key exists in the 
                // dictionary, if it does then replace it
                if(entries[idx].isOccupied() && entries[idx].key_cmp(key_to_write))
                {
                    setEntry(idx, key_to_write, val_to_write, hash_val);
                    return;
                }
                // Check if we should swap
                else if(entries[idx].getPSL() < PSL)
                {
                    std::swap(key_to_write, entries[idx].getKey());
                    std::swap(val_to_write, entries[idx].getValue());
                    std::swap(hash_val, entries[idx].getHash());
                    std::swap(PSL, entries[idx].getPSL());
                }
                
                idx = nextIdx(idx);
                PSL++;
            }
            
            setEntry(idx, key_to_write, val_to_write, hash_val);
            num_entries++;
        }

        bool emplace(K const &key, V &value)
        {            
            int idx = find_index(key);

            if(idx == -1)
            {
                return false;
            }

            value = entries[idx].getValue();
            return true;
        }

        bool remove(K const &key)
        {
            size_t empty_idx = find_index(key);

            if(empty_idx == -1){
                return false;
            }
            
            // Perform backwards shifting
            size_t curr_idx = nextIdx(empty_idx);
            while(entries[curr_idx].isOccupied() && entries[curr_idx].getPSL() > 0)
            {
                moveEntry(curr_idx, empty_idx);
                empty_idx = nextIdx(empty_idx);
                curr_idx = nextIdx(curr_idx);
            }
            entries[empty_idx].setState(EMPTY);
            num_entries--;
            return true;
        }

        size_t getSize(){
            return num_entries;
        }

        size_t getCapacity()
        {
            return capacity;
        }
        
        void printEntries()
        {
            for(int i = 0; i < capacity; i++)
            {
                cout << i << " ";
                if(entries[i].isEmpty())
                {
                    cout << "NULL NULL" << endl;
                }
                else{
                    cout << entries[i].getKey() << " " << entries[i].getValue() << " " << entries[i].getPSL() << endl;
                }
            }
        }

        double calcAvgPSL()
        {
            double avg_PSL = 0.0;
            for(int i = 0; i < capacity; i++)
            {
                if(entries[i].isOccupied())
                {
                    avg_PSL += entries[i].getPSL();
                }
            }

            return avg_PSL/num_entries; 
        }
        
        size_t calcMaxPSL()
        {
            size_t max_PSL = 0;

            for(int i = 0; i < capacity; i++)
            {
                if(entries[i].isOccupied())
                {
                    max_PSL = max(max_PSL, entries[i].getPSL());
                }
            }

            return max_PSL; 
        }
        
        Iterator begin() 
        { 
            size_t curr_idx = 0;
            while(curr_idx < capacity && !entries[curr_idx].isOccupied()){
                curr_idx++;
            }
            return Iterator(&entries[curr_idx], curr_idx, capacity); 
        }

        Iterator end() 
        {
            return Iterator(&entries[capacity], capacity, capacity);
        }
    private: 
        Entry<K, V>* entries;
        size_t num_entries;
        size_t capacity;
        H hash_func;
        double load_factor;
        
        size_t convertHash(size_t hash_val)
        {
            return hash_val & (capacity - 1);
        }

        size_t nextIdx(size_t idx)
        {
            return (idx+1) & (capacity - 1);
        }

        unsigned int nextPowerOf2(unsigned int n)
        {
            n--;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            n++;
            return n;
        }

        void setEntry(size_t const idx, K const &key, V const &value, size_t const hash_val)
        {
            size_t PSL = calcPSL(hash_val, idx);

            entries[idx].setKey(key);
            entries[idx].setValue(value);
            entries[idx].setHash(hash_val);
            entries[idx].setPSL(PSL);
            entries[idx].setState(OCCUPIED);
       
        }

        size_t calcPSL(size_t hash_val, size_t actual_idx)
        {
            size_t expected_idx = convertHash(hash_val);
            return calcDist(expected_idx, actual_idx);
        }

        size_t calcDist(size_t start_idx, size_t end_idx)
        {
            size_t dist = end_idx - start_idx;
            if(dist < 0){
                dist += capacity;
            }
            return dist;
        }

        void moveEntry(size_t src_idx, size_t dst_idx)
        {
            K key = entries[src_idx].getKey();
            V val = entries[src_idx].getValue();
            size_t hash = entries[src_idx].getHash();
            
            entries[dst_idx].setKey(key);
            entries[dst_idx].setValue(val);
            entries[dst_idx].setHash(hash);
            
            size_t new_PSL = calcPSL(entries[src_idx].getHash(), dst_idx);
            entries[dst_idx].setPSL(new_PSL);

            entries[src_idx].setState(EMPTY);
            entries[dst_idx].setState(OCCUPIED);
        }

        size_t find_index(K const &key)
        {
            size_t hash_val = hash_func(key, capacity);
            size_t start_idx = convertHash(hash_val);

            for(size_t i = 0; i < capacity; i++){
                size_t idx = convertHash(start_idx + i);
                // Check if the key exists in the 
                // dictionary, if it does then replace it
                if(entries[idx].isEmpty())
                {
                    return -1;
                }
                else if(entries[idx].isOccupied() && entries[idx].key_cmp(key))
                {
                    return idx;
                }
            }
            return -1;
        }
        
        void reallocate()
        {
            // Calculate new capacity
            size_t old_capacity = capacity;
            capacity = 2*capacity;
            
            // Store old key value pairs
            Entry<K, V>* old_entries = entries;
            
            // Allocate a new array of Entry pointer objects
            entries = new Entry<K, V>[capacity];

            // insert all entries into new_entries
            for(int i = 0; i < old_capacity; i++)
            {
                if(!old_entries[i].isOccupied())
                {
                    continue;
                }
                
                K key_to_write = old_entries[i].getKey();
                V val_to_write = old_entries[i].getValue();

                // Hash the key and get the index
                size_t hash_val = old_entries[i].getHash();
                size_t idx = convertHash(hash_val);
                size_t PSL = 0;
                while(!entries[idx].isEmpty()){
                    // Check if we should swap
                    if(entries[idx].getPSL() < PSL)
                    {
                        std::swap(key_to_write, entries[idx].getKey());
                        std::swap(val_to_write, entries[idx].getValue());
                        std::swap(hash_val, entries[idx].getHash());
                        std::swap(PSL, entries[idx].getPSL());
                    }
                    idx = nextIdx(idx);
                    PSL++;
                }        
                setEntry(idx, key_to_write, val_to_write, hash_val);
            }

            // Remove old entries
            delete old_entries;
        }
};

#endif