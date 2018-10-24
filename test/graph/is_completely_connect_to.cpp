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


int test_graph_is_completely_connect_to(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Create vertex set for g
    OrderedVertexSet g_vertex_set;
    for (size_t v = 0; v < g.get_num_vertices(); v++) g_vertex_set.add_vertex_unsafe(v);

    // For each vertex, check its connectivity
    for (auto idx : g_vertex_set) {

        // ensure "v connected to neighb(v)" is true
        OrderedVertexSet neighb = g.get_neighbors(idx);

        if (!g.is_completely_connect_to(idx,neighb)) {
            std::cout << "Node " << idx << " not connected to its neighborhood." << std::endl;
            error = true;
        }

        // Now check v connected to subsets of its neighborhood
        for (auto v : neighb ) {

            auto neighb_subset = neighb.truncate_before_node(v);
            if (neighb_subset.size() == 0) continue;  // skip empty sets

            if (!g.is_completely_connect_to(idx,neighb_subset)) {
                std::cout << "Node " << g.get_external_vertex_label(idx) << " not connected to a subset of its neighborhood; " << g.get_external_vertex_label(v) << std::endl;
                error = true;
            }

        }

        // Now check v is not connected to any node outside its neighborhood
        OrderedVertexSet neighb_complement = g_vertex_set.set_minus(neighb);

        for (auto v : neighb_complement) {
            OrderedVertexSet temp_set(std::vector<size_t> {v});
            if (g.is_completely_connect_to(idx,temp_set)) {
                std::cout << "Node " << idx << " should not be connected to node"
                          << v << "." << std::endl;
                error = true;
            }
        }

    }

    return error;

}
