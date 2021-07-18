# Open Addressing HashMaps

I recently stumbled across the wild world of hashmaps when I was writing a chapter about how dictionaries are implemented in Python and 
fell down a pretty deep rabbit hole. I found a talk by a very intelligent man named Malte Skarupke about optimizations one could make in 
creating a hashmap that could beat even the speed of C++'s standard library hashmap. Intruiged, I set out to determine whether I myself 
could write a hashmap faster that would rival or potentially even surpass the performance of std::unordered_map.

# std::unordered_map

std::unordered_map uses a technique called chaining to build its hashmap. Essentially 
