#ifndef ENTRY_HPP 
#define ENTRY_HPP

#include <stdlib.h>

enum State
   {
      EMPTY,
      OCCUPIED    
   };

template <typename K, typename V>
class Entry {
public:
    Entry() : key(), val(), hash_val(0), PSL(0), state(EMPTY){}
    
    K& getKey() {
        return key;
    }

    V& getValue() {
        return val;
    }

    size_t& getHash() {
        return hash_val;
    }

    size_t& getPSL() {
        return PSL;
    }

    bool key_cmp(K const &candidate_key){
        return candidate_key == key;
    }
    bool isEmpty(){
        return state == EMPTY;
    }
    bool isOccupied(){
        return state == OCCUPIED;
    }
    State getState() const{
        return state;
    }

    void setKey(const K &new_key){
        key = new_key;
    }
    void setValue(const V &new_val) {
        val = new_val;
    }
    void setHash(const size_t new_hash_val) {
        hash_val = new_hash_val;
    }
    void setState(State new_state){
        state = new_state;
    }
    void setPSL(size_t new_PSL){
        PSL = new_PSL;
    }

    void populate(K const &key, V const &value, size_t const hash_val){
        setKey(key);
        setValue(value);
        setHash(hash_val);
        setState(OCCUPIED);
    }
    
private:
    // key-value pair
    K key;
    V val;
    // Store hash for reallocation
    size_t hash_val;
    // Store probe sequence length
    size_t PSL;
    // Store state
    State state;
};

#endif