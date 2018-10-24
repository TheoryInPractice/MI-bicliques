/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/graph/OrderedVector.h"


int test_orderedvector_vertex_list_operations(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    std::vector<size_t> true_vector = {1, 23, 37, 55};
    OrderedVector ovs(true_vector);

    // test get_vertices()
    std::vector<size_t> computed_vector = ovs.get_vertices();
    if (computed_vector != true_vector) {
        std::cout << "get_vertices returns incorrect array of vertices." << std::endl;
        error = true;
    }

    // test has_vertex()
    if (ovs.has_vertex(100)) {
        std::cout << "has_vertex should be false for vertices not present." << std::endl;
        error = true;
    }
    for (auto x : true_vector) {
        if (!ovs.has_vertex(x)) {
            std::cout << "has_vertex should be true for vertex: " << x << std::endl;
            error = true;
        }
    }

    // test add_vertex_unsafe
    // If vertex present, should not add
    OrderedVector ovs_copy(true_vector);

    // If vertex not present, should add
    ovs_copy.bubble_sort(111); // not present, something should change
    ovs_copy.bubble_sort(1112);
    OrderedVector ground_truth((std::vector<size_t>) {1, 23, 37, 55, 111, 1112});
    if (!(ovs_copy == ground_truth)) {
        std::cout << "Adding new vertices should change the set." << std::endl;
        std::cout << "\tShould be " << ground_truth.to_string() << std::endl;
        std::cout << "\tinstead is " << ovs_copy.to_string() << std::endl;
        error = true;
    }

    return error;

}
