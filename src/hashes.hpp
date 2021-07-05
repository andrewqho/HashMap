#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <unordered_set>

template <typename K>
struct IntHash {
    unsigned long operator()(const K& k) const
    {
        return k;
    }
};

template <typename K>
struct GenericHash
{
    unsigned long operator()(K const& key) const
    {   
        // Instantiate hash function
        std::hash<K> generic_hash;

        // Calculate hash
        unsigned long h = generic_hash(key);
        return h;
    }
};