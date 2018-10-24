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


int test_orderedvertexset_set_intersection(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    OrderedVertexSet set_one((std::vector<size_t>){3,5,7,9});

    // add nothing new
    OrderedVertexSet set_two((std::vector<size_t>){5,7,9});

    // add something at the start
    OrderedVertexSet set_three((std::vector<size_t>){1,5,7,9});

    // add things inbetween
    OrderedVertexSet set_four((std::vector<size_t>){5,6,7,8,9});

    // add things at the end
    OrderedVertexSet set_five((std::vector<size_t>){5,7,8,9,10});

    // Check intersection with a subset
    auto new_set = set_one.set_intersection(set_two);
    if (set_two != new_set) {
        std::cout << "Set intersection with a subset does not produce the subset." << std::endl;
        error = true;
    }

    // Check intersection with a subset plus smaller elements
    new_set = set_one.set_intersection(set_three);
    if (set_two != new_set) {
        std::cout << "Set intersection with a subset plus smaller elements is not correct." << std::endl;
        error = true;
    }

    // Check intersection with a subset plus inbetween elements
    new_set = set_one.set_intersection(set_four);
    if (set_two != new_set) {
        std::cout << "Set intersection with a subset plus inbetween elements is not correct." << std::endl;
        error = true;
    }

    // Check intersection with a subset plus inbetween elements
    new_set = set_one.set_intersection(set_five);
    if (set_two != new_set) {
        std::cout << "Set intersection with a subset plus larger elements is not correct." << std::endl;
        error = true;
    }

    return error;

}
