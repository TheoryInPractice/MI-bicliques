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
int test_editablegraph_delete_node(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // read in Graph object
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // For each node, create a graph copy with just that node deleted
    for (size_t current_vertex = 0; current_vertex < g.get_num_vertices(); current_vertex++){

        // copy EditableGraph from the Graph
        EditableGraph eg(g);

        // delete current node
        auto deleted_neighborhood = eg.get_neighbors(current_vertex);
        eg.delete_node(current_vertex);

        // check vertex_list is correct
        if (eg.has_vertex(current_vertex)) {
            std::cout << "EditableGraph should have deleted vertex " << current_vertex << std::endl;
            error = true;
        }

        // For each neighbor of vertex, check vertex is deleted from neighbor's neighborhood
        for (auto const &iter : deleted_neighborhood) {

            size_t current_neighb = iter.first;

            auto neighborhood_of_neighb = eg.get_neighbors(current_neighb);

            if ( neighborhood_of_neighb.find(current_vertex) != neighborhood_of_neighb.end()) {
                std::cout << "EditableGraph should have deleted current vertex " << current_vertex << " from neighborhood of node "
                          << " current_neighb." << std::endl;
                error = true;
            }
        }

        // check that remaining adjacency info is correct for every vertex pair
        for (size_t idx1 = 0; idx1 < g.get_num_vertices(); idx1++){

            // skip the deleted vertex
            if (idx1 == current_vertex) continue;

            for (size_t idx2 = 0; idx2 < g.get_num_vertices(); idx2++){

                // skip the deleted vertex
                if (idx2 == current_vertex) continue;

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
    }

    return error;

}
