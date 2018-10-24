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


int test_orderedvertexset_set_minus(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    OrderedVertexSet set_one((std::vector<size_t>){3, 5, 7, 9});

    // subtract a subset
    OrderedVertexSet set_two((std::vector<size_t>){5,9});

    // subtract something with no common elements
    OrderedVertexSet set_three((std::vector<size_t>){1,2,4,10});

    // subtract a subset plus extra elements
    OrderedVertexSet set_four((std::vector<size_t>){5,6,8,9});

    // subtract a superset
    OrderedVertexSet set_five((std::vector<size_t>){3,4,5,7,9});


    // Check set minus subset
    auto new_set = set_one.set_minus(set_two);
    OrderedVertexSet true_set((std::vector<size_t>){3,7});
    if (true_set != new_set) {
        std::cout << "Set minus a subset is incorrect." << std::endl;
        error = true;
    }

    // Check set minus irrelevant set
    new_set = set_one.set_minus(set_three);
    if (set_one != new_set) {
        std::cout << "Set minus irrelevant elements should equal the original set." << std::endl;
        error = true;
    }

    // Check set minus irrelevant set plus extra elements
    new_set = set_one.set_minus(set_four);
    if (true_set != new_set) {
        std::cout << "Set minus a subset plus extra elements is incorrect." << std::endl;
        error = true;
    }

    // Check set minus superset
    new_set = set_one.set_minus(set_five);
    auto empty_set = set_one.set_minus(set_one);
    if (empty_set != new_set) {
        std::cout << "Set minus a superset should be empty." << std::endl;
        error = true;
    }

    return error;

}
