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
 * Test that the BicliqueLite functions return the correct partitions.
 */
int test_biclique_get_partitions(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Left and right vertex sets
    std::vector<size_t> true_left((std::vector<size_t>) {1,3});
    std::vector<size_t> true_right((std::vector<size_t>) {2,4,5});
    std::vector<size_t> true_all((std::vector<size_t>) {1,2,3,4,5});

    // Create biclique
    BicliqueLite biclique(true_left, true_right);

    // Check each partition is correct
    if (biclique.get_left() != true_left) {
        std::cout << "BicliqueLite left partition is incorrect" << std::endl;
        error = true;
    }

    if (biclique.get_right() != true_right) {
        std::cout << "BicliqueLite right partition is incorrect" << std::endl;
        error = true;
    }

    if (biclique.get_all_vertices() != true_all) {
        std::cout << "BicliqueLite 'get_all_vertices' is incorrect" << std::endl;
        error = true;
    }

    return error;

}
