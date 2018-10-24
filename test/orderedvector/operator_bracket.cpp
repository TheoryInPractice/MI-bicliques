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


int test_orderedvector_operator_bracket(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // create two sets that are different at each index.
    std::vector<size_t> true_vector = {1,3,5,7,9};
    OrderedVector set_one(true_vector);
    OrderedVector set_two((std::vector<size_t>) {3,5,7,9,10});

    // Compare value returned by operator[] to the values of the input vector
    for ( size_t idx = 0; idx < true_vector.size(); idx++) {
        if (true_vector[idx] != set_one[idx]) {
            std::cout << "Entry OrderedVector[" << idx << "] incorrect." << std::endl;
            error = true;
        }
    }
    // Check that two sets that differ at an index return different values via operator[]
    for ( size_t idx = 0; idx < set_one.size(); idx++) {
        if (set_one[idx] == set_two[idx]) {
            std::cout << "Entry OrderedVector[" << idx << "] incorrect." << std::endl;
            error = true;
        }
    }

    return error;

}
