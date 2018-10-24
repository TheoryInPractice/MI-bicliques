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
 * Test SimpleOCT::simpleoct_bipartite_huge -- tests whether simpleoct can
 * identify when a graph is bipartite, and correctly output left, right,
 * and empty set as oct.
 */
int test_simpleoct_simpleoct_bipartite_huge(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/bipartite_left50right500p0.09.txt";
    Graph use_g(path_to_file, Graph::FILE_FORMAT::adjlist);

    OrderedVertexSet oct_computed, left_computed, right_computed;
    simpleoct(use_g, oct_computed, left_computed, right_computed);


    std::cout << "SimpleOCT output oct set with size" << oct_computed.size() << std::endl;
    std::cout << "left set with size" << left_computed.size() << std::endl;
    std::cout << "right set with size" << right_computed.size() << std::endl;


    // use_g.print_graph();
    // Check the oct set is empty
    if (oct_computed.size() > 0) {
        std::cout << "SimpleOCT output oct set with size" << oct_computed.size() << std::endl;
        std::cout << "left set with size" << left_computed.size() << std::endl;
        std::cout << "right set with size" << right_computed.size() << std::endl;
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

    return error;

}
