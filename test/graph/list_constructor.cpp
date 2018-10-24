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

/**
 * Test the constructor in the class Graph. This constructor takes as inputs a vertex set,
 * an adjacency list, and a hashtable that gives labels for the nodes, and outputs
 * a Graph object with the given adjacency structure and node labels on the set of vertices
 * passed in. In particular, the vertex set can be simply a subset of the vertices
 * in the input adjacency list.
 */
int test_graph_list_constructor(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::vector<std::vector<size_t>> adj_list = { {3,2,1}, {0,2}, {3,0,1}, {0,2} };
    std::unordered_map<size_t,std::string> node_labels = {
        {0, "zero"},
        {1, "one"},
        {2, "two"},
        {3, "three"},
    };
    // Note this vertx set is deliberately only a subset of the vertices in the adjacency list.
    OrderedVertexSet vertex_set((std::vector<size_t>) {0, 1, 3});

    Graph g(vertex_set.get_vertices(), adj_list, node_labels);

    // Check basic graph info
    if (g.get_num_edges() != 2) {
        std::cout << "Num edges should be 2, is " << g.get_num_edges() << std::endl;
        error = true;
    }
    if (g.get_num_vertices() != 3) {
        std::cout << "Num vertices should be 3, is " << g.get_num_edges() << std::endl;
        error = true;
    }

    // Now check correctness of adjacency information
    std::vector<std::vector<size_t>> list_to_check = { {0,1}, {1,0}, {0,2}, {2,0} };

    for ( auto pair : list_to_check ) {

        if (!g.has_edge(pair[0], pair[1])) {
            std::cout << "Graph should have edge " << pair[0]
                      << ", " << pair[1] << std::endl;
            error = true;
        }

    }

    // check edges that shouldn't be there aren't there
    std::vector<std::vector<size_t>> list_to_check2 = { {1,2}, {2,1} };

    for ( auto pair : list_to_check2 ) {

        if (g.has_edge(pair[0], pair[1])) {
            std::cout << "Graph should not have edge " << pair[0]
                      << ", " << pair[1] << std::endl;
            error = true;
        }

    }

    // Check correctness of labels
    // Note: the graph constructed has labels 0,1,2 corresponding
    // to indices 0,1,3 in the vector used to construct the graph.
    // So the labels should be 0: "zero", 1: "one", 2: "three".
    std::vector<std::string> correct_labels = {"zero", "one", "three"};

    for (size_t idx = 0; idx < correct_labels.size(); idx++ ) {

        std::string true_label = correct_labels[idx];
        std::string computed_label = g.get_external_vertex_label(idx);

        if ( true_label != computed_label ) {

            std::cout << "Graph vertex " << idx << " should have label " << true_label << ", not " << computed_label << std::endl;
            error = true;
        }


    }

    return error;

}
