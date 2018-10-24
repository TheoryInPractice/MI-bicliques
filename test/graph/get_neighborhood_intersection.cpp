/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/graph/Graph.h"


int test_graph_get_neighborhood_intersection(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Create test cases
    std::vector<OrderedVertexSet> sets_to_operate = {
        OrderedVertexSet ((std::vector<size_t>) {0}),
        OrderedVertexSet ((std::vector<size_t>) {1,5}),
        OrderedVertexSet ((std::vector<size_t>) {0,3,6}),
        OrderedVertexSet ((std::vector<size_t>) {2,3,5,6}),
        OrderedVertexSet ((std::vector<size_t>) {2,3}),
        OrderedVertexSet ((std::vector<size_t>) {})
    };

    std::vector<OrderedVertexSet> intersections_true = {
        OrderedVertexSet ((std::vector<size_t>) {1,2}),
        OrderedVertexSet ((std::vector<size_t>) {3}),
        OrderedVertexSet ((std::vector<size_t>) {}),
        OrderedVertexSet ((std::vector<size_t>) {4}),
        OrderedVertexSet ((std::vector<size_t>) {1,4}),
        OrderedVertexSet ((std::vector<size_t>) {})
    };

    OrderedVertexSet temp_empty_set( (std::vector<size_t>){} );

    for (size_t idx = 0; idx < sets_to_operate.size(); idx++){

        auto set_to_operate = sets_to_operate[idx];
        std::vector<size_t> intersection_true = intersections_true[idx].get_vertices();
        std::vector<size_t> intersection_computed = g.get_neighborhood_intersection(set_to_operate, temp_empty_set, false);

        if ( intersection_computed != intersection_true ) {
            std::cout << "Set " << idx << " has mistake. Should contain: ";
            for (auto v: intersection_true) { std::cout << v << " ";}
            std::cout << std::endl;

            std::cout << "Instead, contains: ";
            for (auto v: intersection_computed) { std::cout << v << " ";}
            std::cout << std::endl;

            error = true;
        }
    }

    return error;

}
