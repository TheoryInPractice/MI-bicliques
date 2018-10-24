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


int test_graph_subgraph_permutations(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Orderings to try
    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4,5,6},
        {6,1,2,3,4,5,0},
        {6,1,5,2,4,3,0},
        {0,6,2,5,3,4,1},
        {3,1,2,5,6,4,0},
    };


    for (const std::vector<size_t> ordering: orderings) {

        // Subgraph induced by vertex set
        Graph subgraph = g.subgraph(ordering);

        // Test basic graph information
        if (subgraph.get_num_edges() != 10) {
            std::cout << "Num edges should be 10, is " << subgraph.get_num_edges() << std::endl;
            error = true;
        }
        if (subgraph.get_num_vertices() != 7) {
            std::cout << "Num vertices should be 7, is " << subgraph.get_num_vertices() << std::endl;
            error = true;
        }

        // Now check correctness of subgraph.
        // Note this checks that the labels 'node_labels' in graph and
        // subgraph provide a "subgraph isomorphism" from subgraph into graph.
        for (size_t idx = 0; idx < subgraph.get_num_vertices(); idx++) {

            size_t super_idx = ordering[idx];
            OrderedVertexSet sub_neighborhood = subgraph.get_neighbors(idx);
            OrderedVertexSet super_neighborhood = g.get_neighbors(super_idx);

            for (size_t idx2 = 0; idx2 < subgraph.get_num_vertices(); idx2++ ) {

                // Test adjacency matrix is correct using numeric labels
                size_t super_idx2 = ordering[idx2];
                if ( subgraph.has_edge(idx, idx2) != g.has_edge(super_idx,super_idx2) ) {
                    std::cout << "Number label edge " << super_idx << ", " << super_idx2 << " conflicts in graph vs subgraph." << std::endl;
                    error = true;
                }

                // Now test adjacency matrix is correct using string labels
                std::string subg_v_str1 = subgraph.get_external_vertex_label(idx);
                std::string subg_v_str2 = subgraph.get_external_vertex_label(idx2);
                if ( subgraph.has_edge(subg_v_str1, subg_v_str2) != g.has_edge(subg_v_str1,subg_v_str2) ) {
                    std::cout << "Number label edge " << subg_v_str1 << ", " << subg_v_str2 << " conflicts in graph vs subgraph." << std::endl;
                    error = true;
                }

                // Now test Adjacency list information
                if (sub_neighborhood.has_vertex(idx2) != super_neighborhood.has_vertex(super_idx2) ) {
                    std::cout << "Subgraph vertex " << idx << " neighbor " << idx2 << " conflicts with graph verts " << super_idx << " and " << super_idx2 << std::endl;
                    error = true;
                }

            }
        }

    }

    return error;

}
