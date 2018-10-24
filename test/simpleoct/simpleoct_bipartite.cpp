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
#include"../../src/graph/OrderedVertexSet.h"
#include"../../src/algorithms/SimpleOCT.h"

/**
 * Test SimpleOCT::simpleoct_bipartite -- tests whether simpleoct can
 * identify when a graph is bipartite, and correctly output left, right,
 * and empty set as oct.
 */
int test_simpleoct_simpleoct_bipartite(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_bipartite.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Test the code on different permutations of the same graph
    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4},
        {1,2,3,4,0},
        {1,2,4,3,0},
        {0,2,3,4,1}
    };

    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        Graph use_g = g.subgraph(ordering);
        OrderedVertexSet oct_computed, left_computed, right_computed;
        simpleoct(use_g, oct_computed, left_computed, right_computed);

        // Check the oct set is empty
        if (oct_computed.size() > 0) {
            std::cout << "SimpleOCT output incorrect oct set:" << oct_computed.to_string() << std::endl;
            error = true;
        }

        // Check the coloring is valid
        for (auto v: left_computed) {
            OrderedVertexSet this_left = left_computed.set_minus(OrderedVertexSet((std::vector<size_t>){v}));
            if (!use_g.is_completely_independent_from(v, this_left)) {
                std::cout << "SimpleOCT output incorrect on left partition." << std::endl;
                std::cout << "Vertex " << v << " has neighbor in same partition." << std::endl;
                error = true;
            }
        }

        for (auto v: right_computed) {
            OrderedVertexSet this_right = right_computed.set_minus(OrderedVertexSet((std::vector<size_t>){v}));
            if (!use_g.is_completely_independent_from(v, this_right)) {
                std::cout << "SimpleOCT output incorrect on right partition." << std::endl;
                std::cout << "Vertex " << v << " has neighbor in same partition." << std::endl;
                error = true;
            }
        }

    }

    return error;

}
