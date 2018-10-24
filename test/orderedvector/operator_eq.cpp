/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


#include <iostream>
#include <vector>
#include "../../src/graph/OrderedVector.h"


int test_orderedvector_operator_eq(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // create two sets that are equal, and two sets that differ
    OrderedVector set_one((std::vector<size_t>) {1, 3, 5, 7, 9});
    OrderedVector set_two((std::vector<size_t>) {1, 3, 5, 7, 9});

    // differs in content, not size
    OrderedVector set_three((std::vector<size_t>) {1, 3, 5, 7, 10});

    // differs in size
    OrderedVector set_four((std::vector<size_t>) {1, 3, 5, 7, 9, 10});

    if ( set_one != set_two ) {
        std::cout << "Identical sets not identified as equal." << std::endl;
        error = true;
    }

    if (set_one == set_three) {
        std::cout << "Different sets of same size identified as equal." << std::endl;
        error = true;
    }

    if (set_one == set_four) {
        std::cout << "Sets of different size identified as equal." << std::endl;
        error = true;
    }

    return error;

}
