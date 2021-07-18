# Open Addressing HashMaps

I recently stumbled across the wild world of hashmaps when I was writing a chapter about how dictionaries are implemented in Python and 
fell down a pretty deep rabbit hole. I found a talk by a very intelligent man named Malte Skarupke about optimizations one could make in 
creating a hashmap that could beat even the speed of C++'s standard library hashmap. Intruiged, I set out to determine whether I myself 
could write a hashmap faster that would rival or potentially even surpass the performance of std::unordered_map.

# std::unordered_map

## The Implementation
std::unordered_map uses a technique called chaining to build its hashmap. You can think of the underlying structure of std::unordered_map as a static array with buckets of linked lists that hold key-value pairs. When a new key-value pair needs to be inserted into the hashmap, std::unordered_map takes the key, hashes it, and converts the hash into an index. std::unordered_map then goes to its table, finds the index, and checks whether a key or keys already live there. If the index is empty, then the table simply inserts the key-value pair. However, if there are multiple keys, then std::unordered_map will check all of the keys that live there already to determine if the key already exists. If it does, then it only changes the value. If it doesn't, then std::unordered_map will insert the new key-value pair into a chain of other key-value pairs in the form of a linked-list. When a key-value pair needs to be removed

## Pros and Cons

Chaining is a very simple method to ensure that keys are safely kept in the bucket they're assigned to. In addition, since we can grow the linked list to infinite length, technically we can never run out of space in our hashmap. The static array does not need to be grown. However, this does come with some significant drawbacks. In the case that elements are mapped to a small range of indices, there's a lot of wasted space in the hashmap. Cache performance is also not great with linked lists, since linked list nodes are not (necessarily) adjacent in memory. As you iterate along the chain, you run into a bunch of cache misses.

Our goal is to minimize wasted space and improve cache performance of searches with the following hashmap implementations.

# LinearMap

## The Implementation
The first hashmap I implemented uses open addressing with linear probing. The idea of open addressing is that key-value pairs can live in slots that it does not necessarily map to. Suppose you want to insert a key-value pair, and the key hashes to index 21. But in index 21, there already exists a key-value pair. Instead of chaining, what we do is we "probe" for an empty slot
