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


int test_orderedvector_vector_minus_neighborhood(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Check when intersection = input set
    OrderedVector set_to_operate((std::vector<size_t>) {0,1,2,3,4,5,6});
    std::vector<OrderedVector> true_sets = {
        ((std::vector<size_t>) {0,3,4,5,6}),
        ((std::vector<size_t>) {1,4,5,6}),
        ((std::vector<size_t>) {2,3,5,6}),
        ((std::vector<size_t>) {0,2,3,6}),
        ((std::vector<size_t>) {0,1,4}),
        ((std::vector<size_t>) {0,1,2,5}),
        ((std::vector<size_t>) {0,1,2,3,6})
    };
    for (size_t idx = 0; idx < g.get_num_vertices(); idx++){
        OrderedVector set_computed = set_to_operate;
        set_computed.vector_minus_neighborhood(g, idx);
        OrderedVector set_true = true_sets[idx];

        if ( set_computed != set_true ) {
            std::cout << "Subtract neighborhood for vertex " << idx << " has mistake.";
            std::cout << " Should contain: " << set_true.to_string() << std::endl;
            std::cout << "Instead, contains: " << set_computed.to_string() << std::endl;

            error = true;
        }
    }

    // Check when intersection = empty set
    set_to_operate = OrderedVector( (std::vector<size_t>){} );
    for (size_t idx = 0; idx < g.get_num_vertices(); idx++){
        OrderedVector set_true = set_to_operate;
        OrderedVector set_computed = set_to_operate;
        set_computed.vector_minus_neighborhood(g, idx);

        if ( set_computed != set_true ) {
            std::cout << "Subtract neighborhood for vertex " << idx << " has mistake. Should be empty.";
            std::cout << "Instead, contains: ";
            std::cout << set_computed.to_string() << std::endl;

            error = true;
        }
    }

    // Check when intersection = input set
    set_to_operate = OrderedVector ((std::vector<size_t>) {0,1,2,5});
    true_sets = (std::vector<OrderedVector>){
        ((std::vector<size_t>) {0,5}),
        ((std::vector<size_t>) {1,5}),
        ((std::vector<size_t>) {2,5}),
        ((std::vector<size_t>) {0,2}),
        ((std::vector<size_t>) {0,1}),
        ((std::vector<size_t>) {0,1,2,5}),
        ((std::vector<size_t>) {0,1,2})
    };
    for (size_t idx = 0; idx < g.get_num_vertices(); idx++){
        OrderedVector set_computed = set_to_operate;
        set_computed.vector_minus_neighborhood(g, idx);
        OrderedVector set_true = true_sets[idx];

        if ( set_computed != set_true ) {
            std::cout << "Subtract neighborhood for vertex " << idx << " has mistake.";
            std::cout << " Should contain: " << set_true.to_string() << std::endl;
            std::cout << "Instead, contains: " << set_computed.to_string() << std::endl;

            error = true;
        }
    }

    return error;

}
