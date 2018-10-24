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


int test_graph_subgraph(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Get vertex subset
    std::vector<std::string> external_label_list = {"dingo", "badger", "elephant", "guppy", "ferret"};
    std::vector<size_t> internal_subset_list;
    for (auto label : external_label_list) {

        size_t internal_label;

        if (g.get_internal_vertex_label(label, internal_label)) {
            internal_subset_list.push_back(internal_label);
        }

    }

    sort(internal_subset_list.begin(), internal_subset_list.end());

    OrderedVertexSet subset_vertices(internal_subset_list);

    // Subgraph induced by vertex set
    auto subgraph = g.subgraph(subset_vertices);

    // Test basic graph information
    if (subgraph.get_num_edges() != 6) {
        std::cout << "Num edges should be 6, is " << subgraph.get_num_edges() << std::endl;
        error = true;
    }
    if (subgraph.get_num_vertices() != 5) {
        std::cout << "Num vertices should be 5, is " << subgraph.get_num_edges() << std::endl;
        error = true;
    }

    // Now check correctness of subgraph.
    // Note this checks that the labels 'node_labels' in graph and
    // subgraph provide a "subgraph isomorphism" from subgraph into graph.
    for (size_t idx = 0; idx < subgraph.get_num_vertices(); idx++) {

        std::string ex_label1 = subgraph.get_external_vertex_label(idx);

        for (size_t idx2 = 0; idx2 < subgraph.get_num_vertices(); idx2++ ) {

            std::string ex_label2 = subgraph.get_external_vertex_label(idx2);

            if ( subgraph.has_edge(ex_label1,ex_label2) != g.has_edge(ex_label1,ex_label2) ) {

                std::cout << "Edge " << ex_label1 << ", " << ex_label2 << " conflicts in graph vs subgraph." << std::endl;
                error = true;
            }
        }

    }

    return error;

}
