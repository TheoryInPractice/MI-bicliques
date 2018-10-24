/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<sstream>
#include<vector>
#include"../../src/graph/Graph.h"
#include"../../src/graph/OrderedVertexSet.h"
#include "../../src/algorithms/DiasGeneral.h"

/**
 * Test DiasGeneral::lex_least_biclique
 */
int test_diasgeneral_lex_least_biclique(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Hard code starting points
    std::vector<OrderedVertexSet> input_sets = {
        OrderedVertexSet ((std::vector<size_t>) {0}),
        OrderedVertexSet ((std::vector<size_t>) {1}),
        OrderedVertexSet ((std::vector<size_t>) {2}),
        OrderedVertexSet ((std::vector<size_t>) {3}),
    };

    // Hard code bicliques containing each.
    std::vector<BicliqueLite> set_of_bicliques_true = {
        BicliqueLite(OrderedVertexSet ((std::vector<size_t>) {0,3}),
                 OrderedVertexSet ((std::vector<size_t>) {1})),
        BicliqueLite(OrderedVertexSet ((std::vector<size_t>) {0,3}),
                 OrderedVertexSet ((std::vector<size_t>) {1})),
        BicliqueLite(OrderedVertexSet ((std::vector<size_t>) {0,4}),
                 OrderedVertexSet ((std::vector<size_t>) {2})),
        BicliqueLite(OrderedVertexSet ((std::vector<size_t>) {3}),
                 OrderedVertexSet ((std::vector<size_t>) {0,1})),
    };

    for (size_t idx = 0; idx < input_sets.size(); idx++){

        OrderedVertexSet empty_set;
        auto this_set = input_sets[idx];
        auto biclique_true = set_of_bicliques_true[idx];

        // want to test function with both orders:
        // lex_least_biclique(x,y) and lex_least_biclique(y,x)
        std::vector<OrderedVertexSet> partitions = {empty_set, this_set};
        for (int j : {0,1}) {

            auto set1 = partitions[j];
            auto set2 = partitions[1-j];

            auto biclique_computed = lex_least_biclique(g, set1.get_vertices(), set2.get_vertices());

            // Check every BicliqueLite found is actually in the true set
            if (biclique_computed.to_string() != biclique_true.to_string()){
                std::cout << "Testing case: " << idx << ", swap: " << j << std::endl;
                std::cout << "lex_least_biclique wrong on " << biclique_true.to_string() << std::endl;
                std::cout << "Instead finds " << biclique_computed.to_string() << std::endl;

                std::cout << "Starting set1: ";
                for (const auto v: set1) std::cout << v << " ";
                std::cout << std::endl;

                std::cout << "Starting set2: ";
                for (const auto v: set2) std::cout << v << " ";
                std::cout << std::endl;

                std::cout << "Full computed:" << std::endl;
                std::cout << "Left: ";
                OrderedVertexSet left = biclique_computed.get_left();
                for (const auto v: left) std::cout << v << " ";
                std::cout << std::endl << "Right: ";
                OrderedVertexSet right = biclique_computed.get_right();
                for (const auto v: right) std::cout << v << " ";
                std::cout << std::endl << std::endl;

                error=true;
            }
        }

    }

    return error;

}
