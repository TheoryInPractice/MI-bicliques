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


int test_graph_get_neighbors(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Test basic graph information
    if (g.get_num_edges() != 10) {
        std::cout << "Num edges should be 10, is " << g.get_num_edges() << std::endl;
        error = true;
    }
    if (g.get_num_vertices() != 7) {
        std::cout << "Num vertices should be 7, is " << g.get_num_edges() << std::endl;
        error = true;
    }

    // Now check correctness of function
    for (size_t idx = 0; idx < g.get_num_vertices(); idx++) {

        OrderedVertexSet neighborhood = g.get_neighbors(idx);

        for (size_t idx2 = 0; idx2 < g.get_num_vertices(); idx2++ ) {
            if ( neighborhood.has_vertex(idx2) != g.has_edge(idx,idx2) ) {

                std::cout << "Vertex " << idx2 << " and vertex " << idx << " have conflicting neighborhood info." << std::endl;
                error = true;
            }
        }

    }

    return error;

}
