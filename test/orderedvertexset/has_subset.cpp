/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/graph/OrderedVertexSet.h"


int test_orderedvertexset_has_subset(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    OrderedVertexSet input_set((std::vector<size_t>){2,4,5,6,7,9});

    // range within, subset
    OrderedVertexSet dummy_subset_one((std::vector<size_t>){2,9});
    if (!input_set.has_subset(dummy_subset_one)) {
        std::cout << "Set should contain subset {2, 9}." << std::endl;
        error = true;
    }
    //range within but not subset
    OrderedVertexSet dummy_subset_two((std::vector<size_t>){2,8,9});
    if (input_set.has_subset(dummy_subset_two)) {
        std::cout << "Set should not contain {2, 8, 9}." << std::endl;
        error = true;
    }

    // range not within
    OrderedVertexSet dummy_subset_three((std::vector<size_t>){1,9});
    if (input_set.has_subset(dummy_subset_three)) {
        std::cout << "Set should not contain set not within range." << std::endl;
        error = true;
    }
    OrderedVertexSet dummy_subset_four((std::vector<size_t>){2,10});
    if (input_set.has_subset(dummy_subset_four)) {
        std::cout << "Set should not contain set not within range." << std::endl;
        error = true;
    }

    // should contain self
    OrderedVertexSet set_copy = input_set;
    if ( !input_set.has_subset(set_copy) ) {
        std::cout << "Set should contain itself." << std::endl;
        error = true;
    }

    return error;

}
