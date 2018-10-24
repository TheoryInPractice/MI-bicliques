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
 * Test that the BicliqueLite equality comparison operator functions correctly. Two bicliques are considered equivalent if
 * and only if they have exactly the same vertices.
 */
int test_biclique_operator_eq(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Left and right vertex sets
    std::vector<size_t> left1((std::vector<size_t>) {1, 2});
    std::vector<size_t> right1((std::vector<size_t>) {3, 4});
    std::vector<size_t> right2((std::vector<size_t>) {3, 4, 5});
    std::vector<size_t> right3((std::vector<size_t>) {3, 5});

    // Create bicliques
    BicliqueLite base(left1, right1);
    BicliqueLite eq(left1, right1);
    BicliqueLite neq_size(left1, right2);
    BicliqueLite neq_element(left1, right3);

    if (base != eq) {
        std::cout << "Bicliques Base and EQ should be the same" << std::endl;
        error = true;
    }

    if (base == neq_size) {
        std::cout << "Bicliques Base and NEQ_Size have different sizes and should not be the same." << std::endl;
        error = true;
    }

    if (base == neq_element) {
        std::cout << "Bicliques Base and NEQ_Element have different elements and should not be the same." << std::endl;
        error = true;
    }

    // Check reverses work:
    // Create bicliques
    BicliqueLite reverse_base(right1, left1);
    BicliqueLite reverse_eq(right1, left1);
    BicliqueLite reverse_neq_size(right2, left1);
    BicliqueLite reverse_neq_element(right3, left1);
    if (reverse_base != base) {
        std::cout << "Bicliques Base and EQ should be the same. (reverse)" << std::endl;
        error = true;
    }

    if (base == reverse_neq_size) {
        std::cout << "Bicliques Base and NEQ_Size have different sizes and should not be the same. (reverse)" << std::endl;
        error = true;
    }

    if (reverse_base == reverse_neq_element) {
        std::cout << "Bicliques Base and NEQ_Element have different elements and should not be the same. (reverse)" << std::endl;
        error = true;
    }

    return error;

}
