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
#include"../../src/graph/EditableGraph.h"

/**
 * Test the constructor in the class EditableGraph. This constructor takes as
 * input a Graph object, and checks that the adjacency structure is correctly
 * read in as an EditableGraph.
 */
int test_editablegraph_constructor(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // read in Graph object
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // create EditbaleGraph from the Graph
    EditableGraph eg(g);

    // Check basic graph info
    if (eg.size() != g.get_num_vertices()) {
        std::cout << "Num vertices should be " << g.get_num_vertices()
        << " but is " << eg.size() << std::endl;
        error = true;
    }

    // Now check correctness of adjacency information
    for (size_t idx1 = 0; idx1 < g.get_num_vertices(); idx1++){
        for (size_t idx2 = 0; idx2 < g.get_num_vertices(); idx2++){

            bool actual_edge = g.has_edge(idx1,idx2);

            auto current_neighb = eg.get_neighbors(idx1);
            bool current_edge = false;
            if (current_neighb.find(idx2) != current_neighb.end()) current_edge = current_neighb[idx2];

            if ( current_edge != actual_edge) {
                std::cout << "EditableGraph conflicts on edge " << idx1
                          << ", " << idx2 << std::endl;
                error = true;
            }

        }
    }

    return error;

}
