/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/graph/OrderedVector.h"


int test_orderedvector_has_vertex(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    OrderedVector input_set((std::vector<size_t>){2,4,5,6,7,9});
    std::vector<size_t> excluded_set = {0,1,3,8,10,100};

    // Check all elements inside vector
    for (size_t idx = 0; idx < input_set.size(); idx++){
        size_t dummy = input_set[idx];
        if (!input_set.has_vertex(dummy)) {
            std::cout << "OrderedVector should, but does not contain vertex " << dummy << std::endl;
            error = true;
        }
    }

    // Check elements not in vector
    for (size_t element: excluded_set){
        if (input_set.has_vertex(element)) {
            std::cout << "OrderedVector should not, but does contain vertex " << element << std::endl;
            error = true;
        }
    }

    return error;

}
