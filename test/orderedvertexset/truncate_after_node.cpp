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


int test_orderedvertexset_truncate_after_node(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    OrderedVertexSet set_test( (std::vector<size_t>) {3, 5, 7, 9, 10, 13, 100} );

    // Check that truncating before an irrelevant vertex returns an empty set
    size_t test_vertex = 12; // not in vector v
    OrderedVertexSet computed_set = set_test.truncate_after_node(test_vertex);

    OrderedVertexSet empty_set;
    if (empty_set != computed_set) {
        std::cout << "Truncating before irrelevant vertex should return empty set." << std::endl;
        error = true;
    }

    // Check that truncating before first vertex returns an empty set
    test_vertex = 3;
    computed_set = set_test.truncate_after_node(test_vertex);

    if (empty_set != computed_set) {
        std::cout << "Truncating before first vertex should return empty set." << std::endl;
        error = true;
    }

    // Check that truncating before particular vertex returns correct result
    test_vertex = 9;
    computed_set = set_test.truncate_after_node(test_vertex);
    OrderedVertexSet true_set((std::vector<size_t>) {3, 5, 7});

    if (true_set != computed_set) {
        std::cout << "Truncating before a vertex yields incorrect set." << std::endl;
        error = true;
    }

    return error;

}
