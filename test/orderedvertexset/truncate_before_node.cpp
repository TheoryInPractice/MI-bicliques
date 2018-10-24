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


int test_orderedvertexset_truncate_before_node(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    OrderedVertexSet set_test( (std::vector<size_t>) {3, 5, 7, 9, 10, 13, 100} );

    // Check that truncating at an irrelevant vertex returns an empty set
    size_t test_vertex = 12; // not in vector v
    OrderedVertexSet computed_set = set_test.truncate_before_node(test_vertex);

    OrderedVertexSet empty_set;
    if (empty_set != computed_set) {
        std::cout << "Truncating at irrelevant vertex should return empty set." << std::endl;
        error = true;
    }

    // Check that truncating at last vertex returns an empty set
    test_vertex = 100;
    computed_set = set_test.truncate_before_node(test_vertex);

    if (empty_set != computed_set) {
        std::cout << "Truncating at last vertex should return empty set." << std::endl;
        error = true;
    }

    // Check that truncating at particular vertex returns correct result
    test_vertex = 9;
    computed_set = set_test.truncate_before_node(test_vertex);
    OrderedVertexSet true_set((std::vector<size_t>) {10, 13, 100});

    if (true_set != computed_set) {
        std::cout << "Truncating at a vertex yields incorrect set." << std::endl;
        error = true;
    }

    return error;

}
