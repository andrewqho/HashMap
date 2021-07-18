#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <math.h>   
#include <unordered_map>

bool is_equal(HashMap<int, int, GenericHash<int> > &hmap, std::unordered_map<int, int> &umap){

    for (auto i : hmap){
        int key = i.getKey();
        int value = i.getValue();

        assert(umap.count(key));
        assert(umap.at(key) == value);
    }

    for (auto i: umap){
        int key = i.first;
        int value = i.second;

        int candidate_value;
        assert(hmap.get(key, candidate_value));
        assert(candidate_value == value);
    }

    return true;
}