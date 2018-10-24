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


int test_orderedvector_vector_intersect_neighborhood(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Check when intersection = input set
    OrderedVector set_to_operate((std::vector<size_t>) {0,1,2,3,4,5,6});

    for (size_t idx = 0; idx < g.get_num_vertices(); idx++){
        OrderedVector intersection_true(g.get_neighbors_vector(idx));
        OrderedVector intersection_computed = set_to_operate;
        intersection_computed.vector_intersect_neighborhood(g, idx);

        if ( intersection_computed != intersection_true ) {
            std::cout << "Neighborhood intersection for vertex " << idx << " has mistake. Should contain: ";
            std::cout << intersection_true.to_string() << std::endl;

            std::cout << "Instead, contains: ";
            std::cout << intersection_computed.to_string() << std::endl;

            error = true;
        }
    }

    // Check when intersection = empty set
    set_to_operate = OrderedVector( (std::vector<size_t>){} );
    for (size_t idx = 0; idx < g.get_num_vertices(); idx++){
        OrderedVector intersection_true = set_to_operate;
        OrderedVector intersection_computed = set_to_operate;
        intersection_computed.vector_intersect_neighborhood(g, idx);

        if ( intersection_computed != intersection_true ) {
            std::cout << "Neighborhood intersection for vertex " << idx << " has mistake. Should be empty.";
            std::cout << "Instead, contains: ";
            std::cout << intersection_computed.to_string() << std::endl;

            error = true;
        }
    }

    // Check on specific subset
    set_to_operate = OrderedVector((std::vector<size_t>) {0,1,2,6});
    std::vector<OrderedVector> true_intersections = {
        ((std::vector<size_t>) {1,2}),
        ((std::vector<size_t>) {0,2}),
        ((std::vector<size_t>) {0,1}),
        ((std::vector<size_t>) {1}),
        ((std::vector<size_t>) {2,6}),
        ((std::vector<size_t>) {6}),
        ((std::vector<size_t>) {})
    };

    for (size_t idx = 0; idx < g.get_num_vertices(); idx++){
        OrderedVector intersection_computed = set_to_operate;
        intersection_computed.vector_intersect_neighborhood(g, idx);
        OrderedVector intersection_true = true_intersections[idx];

        if ( intersection_computed != intersection_true ) {
            std::cout << "Neighborhood intersection for vertex " << idx << " has mistake.";
            std::cout << " Should contain: " << intersection_true.to_string() << std::endl;
            std::cout << "Instead, contains: " << intersection_computed.to_string() << std::endl;

            error = true;
        }
    }


    return error;

}
