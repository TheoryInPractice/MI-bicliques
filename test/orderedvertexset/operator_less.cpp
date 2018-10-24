/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


#include <iostream>
#include <vector>
#include "../../src/graph/OrderedVertexSet.h"


int test_orderedvertexset_operator_less(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // create two sets that are equal, and two sets that differ
    OrderedVertexSet set_one((std::vector<size_t>) {1, 3, 5, 7, 9});
    OrderedVertexSet set_two((std::vector<size_t>) {1, 3, 5, 7, 9});

    // differs in content, not size
    OrderedVertexSet set_three((std::vector<size_t>) {1, 3, 5, 7, 10});

    // differs in size
    OrderedVertexSet set_four((std::vector<size_t>) {1, 3, 5, 7, 9, 10});
    OrderedVertexSet empty_set1((std::vector<size_t>) {});
    OrderedVertexSet empty_set2((std::vector<size_t>) {});

    if ( set_one < set_two ) {
        std::cout << "Identical set identified as less than." << std::endl;
        error = true;
    }
    if ( set_two < set_one ) {
        std::cout << "Identical set identified as less than." << std::endl;
        error = true;
    }

    if (!(set_one < set_three)) {
        std::cout << "Lesser set not identified as less than." << std::endl;
        error = true;
    }

    if (!(set_one < set_four)) {
        std::cout << "Subset not identified as less than superset." << std::endl;
        error = true;
    }

    if (set_one < empty_set1) {
        std::cout << "Nothing should be less than the empty set." << std::endl;
        error = true;
    }

    if (empty_set2 < empty_set1) {
        std::cout << "Empty set identified as less than the empty set." << std::endl;
        error = true;
    }

    if (set_four < set_two) {
        std::cout << "Super set identified as less than subset." << std::endl;
        error = true;
    }

    return error;

}
