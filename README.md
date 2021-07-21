<script type="text/x-mathjax-config">
MathJax.Hub.Config({
tex2jax: {
inlineMath: [['$','$'], ['\\(','\\)']],
processEscapes: true},
jax: ["input/TeX","input/MathML","input/AsciiMath","output/CommonHTML"],
extensions: ["tex2jax.js","mml2jax.js","asciimath2jax.js","MathMenu.js","MathZoom.js","AssistiveMML.js", "[Contrib]/a11y/accessibility-menu.js"],
TeX: {
extensions: ["AMSmath.js","AMSsymbols.js","noErrors.js","noUndefined.js"],
equationNumbers: {
autoNumber: "AMS"
}
}
});
</script>

# Open Addressing HashMaps

I recently stumbled across the wild world of hashmaps when I was writing a chapter about how dictionaries are implemented in Python and fell down a pretty deep rabbit hole. Intruiged, I set out to determine whether I myself could write a hashmap faster that would rival or potentially even surpass the performance of std::unordered_map.

# std::unordered_map

## The Implementation
std::unordered_map uses a technique called chaining to build its hashmap. You can think of the underlying structure of std::unordered_map as a static array with buckets of linked lists that hold key-value pairs. When a new key-value pair needs to be inserted into the hashmap, std::unordered_map takes the key, hashes it, and converts the hash into an index. std::unordered_map then goes to its table, finds the index, and checks whether a key or keys already live there. If the index is empty, then the table simply inserts the key-value pair. However, if there are multiple keys, then std::unordered_map will check all of the keys that live there already to determine if the key already exists. If it does, then it only changes the value. If it doesn't, then std::unordered_map will insert the new key-value pair into a chain of other key-value pairs in the form of a linked-list. When a key-value pair needs to be removed, the map iterates over that bucket's linked list and splices out the key-value pair if it is found.

## Pros and Cons

Chaining is a very simple method to ensure that keys are safely kept in the bucket they're assigned to. In addition, since we can grow the linked list to infinite length, technically we can never run out of space in our hashmap. The static array does not need to be grown. However, this does come with some significant drawbacks. In the case that elements are mapped to a small range of indices, there's a lot of wasted space in the hashmap. Cache performance is also not great with linked lists, since linked list nodes are not (necessarily) adjacent in memory. As you iterate along the chain, you run into a bunch of cache misses bouncing across memory addresses. The Big-O of insertions, removals, and look-ups for chaining are all $O(N)$, where $N$ is the number of entries in the hashmap.

Our goal is to minimize wasted space and improve cache performance of searches with the following hashmap implementations.

# LinearMap

## The Implementation
The first hashmap I implemented uses open addressing with linear probing. The idea of open addressing is that key-value pairs can live in slots that it does not necessarily map to. Suppose you want to insert a key-value pair, and the key hashes to index 21. But in index 21, there already exists a key-value pair. Instead of chaining, what we do is we "probe" for an empty slot and stick it in there instead. When we want to look-up a value, we go to the initial bucket that the key hashes to and check if that slot contains the key. If the slot doesn't, then we continue to probe the same way we did looking for an empty slot. 

Normally, I've seen resources online use tombstones when implementing pure linear open-addressed hashmaps. Tombstones are markers that denote entries that have been used

## Theoretical Performance

Since all of the entries are placed next to one another in a flat array, this means that cache performance should be better. However, with tombstones, it's possible that interleaving insertions and removals results in longer probe sequences than following a linked list. In fact, the worst-case scenario for look-ups, insertions, and removals are actually $O(M)$, where $M$ is the capacity of the hashmap. Since we necessarily have that $M > N$, the Big-O of linear hashmaps are actually worse than that of the chained variety.


# Robinhood Map



# Benchmark Tests

## Simple Insertion

This test generates a vector of keys from 0 to $K$, shuffles them into a random order, then records the time it takes to insert all of the keys into the hashmap.

## Simple Look-Up

This test generates a vector of keys from 0 to $K$, shuffles them into a random order, and then inserts the keys into the hashmap. Then it shuffles the keys into a random order again and records the time it takes to look up all of the keys in the vector.

## Simple Removal

This test generates a vector of keys from 0 to $K$, shuffles them into a random order, then inserts the keys into the hashmap. Then it shuffles the keys into a random order again and records the time it takes to remove all of the keys in the vector.

## Insertions with Removals

This test generates a vector of keys from 0 to $K$, shuffles them into a random order, then inserts the keys into the hashmap. Then it shuffles half of the randomized keys into a random order again and removes those keys from the hashmap. Finally, all the keys are shuffled into a random order and the test records the time it takes to insert all of the keys in the vector.

## Look-Ups with Removals

This test generates a vector of keys from 0 to $K$, shuffles them into a random order, then inserts the keys into the hashmap. Then it shuffles half of the randomized keys into a random order again and removes those keys from the hashmap. Finally, all the keys are shuffled into a random order and the test records the time it takes to look-up all of the keys in the vector.

# Results