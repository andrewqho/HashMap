#include <stdlib.h>

template <typename K, typename V>
class Entry {
public:
    Entry(K const &key, V const &val, size_t const hash_val) :
        key(key), val(val), hash_val(hash_val), empty(false){
    }

    K getKey() const {
        return key;
    }

    V getValue() const {
        return val;
    }

    size_t getHash() const {
        return hash_val;
    }

    bool isEmpty() const{
        return empty;
    }

    void setKey(K new_key){
        key = new_key;
    }

    void setValue(V new_val) {
        val = new_val;
    }

    void setHash(size_t new_hash_val) {
        hash_val = new_hash_val;
    }
    void setEmpty(bool state){
        empty = state;
    }

private:
    // key-value pair
    K key;
    V val;
    // Store hash for reallocation
    size_t hash_val;
    // Store empty state
    bool empty;
};