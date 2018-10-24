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


int test_graph_is_completely_independent_from(int argc, char ** argv) {

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

        OrderedVertexSet just_v((std::vector<size_t>){idx});

        // ensure "v independent from complement_neighb(v)" is true
        // OrderedVertexSet neighb = g.get_neighbors(idx);
        OrderedVertexSet neighb_complement = g_vertex_set.set_minus(g.get_neighbors(idx));
        neighb_complement = neighb_complement.set_minus(just_v);

        if (!g.is_completely_independent_from(idx,neighb_complement)) {
            std::cout << "Node " << idx << " not independent from complement of its neighborhood." << std::endl;
            error = true;
        }

        // Now check v independent from subsets of the complement of its neighborhood
        for (auto u : neighb_complement ) {

            auto neighb_complement_subset = neighb_complement.truncate_before_node(u);
            if (neighb_complement_subset.size() == 0) continue;  // skip empty sets

            if (!g.is_completely_independent_from(idx,neighb_complement_subset)) {
                std::cout << "Node " << g.get_external_vertex_label(idx) << " not connected to a subset of its neighborhood; " << g.get_external_vertex_label(u) << std::endl;
                error = true;
            }

        }

        // Now check idx is not independent from any node inside its neighborhood
        for (auto v : g.get_neighbors(idx)) {

            OrderedVertexSet temp_set(std::vector<size_t> {v});

            if (g.is_completely_independent_from(idx,temp_set)) {
                std::cout << "Node " << idx << " should not be connected to node"
                          << v << "." << std::endl;
                error = true;
            }

        }

    }

    return error;

}
