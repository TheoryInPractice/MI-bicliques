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
 * Test that the BicliqueLite serialization function outputs correct string.
 */
int test_biclique_to_string(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Left and right vertex sets
    OrderedVertexSet true_left((std::vector<size_t>) {1,3});
    OrderedVertexSet true_right((std::vector<size_t>) {2,4,5});

    // Create BicliqueLite
    BicliqueLite BicliqueLite(true_left.get_vertices(), true_right.get_vertices());

    // Check string
    if (BicliqueLite.to_string() != (std::string) "1,2,3,4,5,") {
        std::cout << "BicliqueLite string should be '1,2,3,4,5,' not " << BicliqueLite.to_string() << std::endl;
        error = true;
    }

    return error;

}
