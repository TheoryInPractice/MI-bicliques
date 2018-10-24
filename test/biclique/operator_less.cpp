/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


#include <iostream>
#include <vector>
#include "../../src/graph/BicliqueLite.h"
#include "../../src/graph/OrderedVertexSet.h"


/**
 * Test that the BicliqueLite less-than comparison operator is correct.
 * The less than operator is the lexicographical ordering operator.
 */
int test_biclique_operator_less(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Left and right vertex sets
    std::vector<size_t> left1((std::vector<size_t>) {1, 2});
    std::vector<size_t> right1((std::vector<size_t>) {3, 4});
    std::vector<size_t> right2((std::vector<size_t>) {3, 4, 5});
    std::vector<size_t> right3((std::vector<size_t>) {3, 5});

    // Create bicliques
    BicliqueLite smallest(left1, right1);
    BicliqueLite largest(left1, right3);
    BicliqueLite superset(left1, right2);

    if (!(smallest < largest)) {
        std::cout << "Smallest biclique not less than largest." << std::endl;
        error = true;
    }

    if (smallest < smallest) {
        std::cout << "BicliqueLite should not be less than itself." << std::endl;
        error = true;
    }

    if (!(smallest < superset)) {
        std::cout << "BicliqueLite should be less than its superset." << std::endl;
        error = true;
    }

    if (!(superset < largest)) {
        std::cout << "BicliqueLite should be less than largest biclique." << std::endl;
        error = true;
    }

    // Now check with reversed
    BicliqueLite reverse_smallest(right1, left1);
    BicliqueLite reverse_largest(right3, left1);
    BicliqueLite reverse_superset(right2, left1);

    if (!(reverse_smallest < largest)) {
        std::cout << "Smallest biclique not less than largest (reverse)." << std::endl;
        error = true;
    }

    if (reverse_smallest < smallest) {
        std::cout << "BicliqueLite should not be less than itself (reverse)." << std::endl;
        error = true;
    }

    if (!(smallest < reverse_superset)) {
        std::cout << "BicliqueLite should be less than its superset (reverse)." << std::endl;
        error = true;
    }

    if (!(superset < reverse_largest)) {
        std::cout << "BicliqueLite should be less than largest biclique (reverse)." << std::endl;
        error = true;
    }

    return error;

}
