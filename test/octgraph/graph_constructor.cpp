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
#include"../../src/graph/OctGraph.h"


int test_octgraph_graph_constructor(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Set up octgraph
    OrderedVertexSet oct_partition((std::vector<size_t>){2,4});
    OrderedVertexSet left_partition((std::vector<size_t>){0,3,6});
    OrderedVertexSet right_partition((std::vector<size_t>){1,5});

    OctGraph og(g,oct_partition,left_partition,right_partition);

    // Check basic graph info
    if (og.get_num_edges() != 10) {
        std::cout << "Num edges should be 10, is " << og.get_num_edges() << std::endl;
        error = true;
    }
    if (og.get_num_vertices() != 7) {
        std::cout << "Num vertices should be 7, is " << og.get_num_edges() << std::endl;
        error = true;
    }

    // Now check correctness of adjacency information
    // First hard code correct info.
    std::unordered_map<std::string, std::unordered_map<std::string, bool>> adj_map = {
        {"aardvark", std::unordered_map<std::string, bool> {
                    {"capybara", true},
                    {"ferret", true}
                   }
        },
        {"capybara", std::unordered_map<std::string, bool> {
                    {"aardvark", true},
                    {"elephant", true},
                    {"ferret", true}
                   }
        },
        {"ferret", std::unordered_map<std::string, bool> {
                    {"aardvark", true},
                    {"capybara", true},
                    {"dingo", true}
                   }
        },
        {"badger", std::unordered_map<std::string, bool> {
                    {"dingo", true},
                    {"elephant", true},
                    {"guppy", true}
                   }
        },
        {"dingo", std::unordered_map<std::string, bool> {
                    {"badger", true},
                    {"elephant", true},
                    {"ferret", true},
                    {"guppy", true}
                   }
        },
        {"guppy", std::unordered_map<std::string, bool> {
                    {"badger", true},
                    {"dingo", true}
                   }
        },
        {"elephant", std::unordered_map<std::string, bool> {
                    {"badger", true},
                    {"capybara", true},
                    {"dingo", true}
                   }
        }
    };
    // Encodes this adjacency list:
    // aardvark capybara ferret
    // capybara aardvark elephant ferret
    // ferret aardvark capybara dingo
    // badger dingo elephant guppy
    // dingo badger elephant ferret guppy
    // guppy badger dingo
    // elephant badger capybara dingo

    // Check correctness of adjacency matrix
    for ( size_t idx1 = 0; idx1 < og.get_num_vertices(); idx1++ ) {

        for ( size_t idx2 = 0; idx2 < og.get_num_vertices(); idx2++ ) {

            std::string l1 = og.get_external_vertex_label(idx1);
            std::string l2 = og.get_external_vertex_label(idx2);

            // Check adj mat
            bool is_edge_in_adj_map = false;
            if (adj_map[l1].find(l2) != adj_map[l1].end()) {
                is_edge_in_adj_map = true;
            }
            if ( og.has_edge(idx1, idx2) != is_edge_in_adj_map ) {
                std::cout << "Read in octgraph conflicts at edge " << l1 << ", " << l2 << std::endl;
                error = true;
            }

        }

    }

    // Now check proper sorting of adj OrderedVertexSets
    for ( size_t idx1 = 0; idx1 < og.get_num_vertices(); idx1++ ) {

        OrderedVertexSet v_neighborhood = og.get_neighbors(idx1);
        size_t previous_int = 0;
        for (auto cur_int : v_neighborhood) {

            if (previous_int > cur_int) {
                std::cout << "Neighborhood of " << og.get_external_vertex_label(idx1) << " unordered." << std::endl;
                std::cout << "\t " << previous_int << ", " << cur_int << std::endl;
                error = true;
            }

            previous_int = cur_int;

        }
    }

    // Check neighborhood information
    // Now check proper sorting of adj OrderedVertexSets
    for ( size_t idx1 = 0; idx1 < og.get_num_vertices(); idx1++ ) {

        std::string l1 = og.get_external_vertex_label(idx1);
        OrderedVertexSet v_neighborhood = og.get_neighbors(idx1);

        for ( size_t idx2 = 0; idx2 < og.get_num_vertices(); idx2++ ) {

            std::string l2 = og.get_external_vertex_label(idx2);

            if (og.has_edge(idx1, idx2)!=v_neighborhood.has_vertex(idx2)) {
                std::cout << "Read in octgraph conflicts at edge " << l1 << ", " << l2 << std::endl;
                error = true;
            }

        }

    }

    return error;

}
