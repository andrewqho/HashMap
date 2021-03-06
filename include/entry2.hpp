#ifndef ENTRY2_HPP 
#define ENTRY2_HPP

#include <stdlib.h>

enum State2
   {
      CLEAN,
      IN_USE,
      DIRTY      
   };

template <typename K, typename V>
class Entry2 {
public:
    Entry2() : key(), val(), hash_val(0), PSL(0), state(CLEAN){}
    
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
    bool isDirty(){
        return state == DIRTY;
    }
    bool isClean(){
        return state == CLEAN;
    }
    bool isInUse(){
        return state == IN_USE;
    }
    State2 getState() const{
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
    void setState(State2 new_state){
        state = new_state;
    }
    void setPSL(size_t new_PSL){
        PSL = new_PSL;
    }

    void clear(){
        state = DIRTY;
    }
    void populate(K const &key, V const &value, size_t const hash_val){
        setKey(key);
        setValue(value);
        setHash(hash_val);
        setState(IN_USE);
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
    State2 state;
};

#endif