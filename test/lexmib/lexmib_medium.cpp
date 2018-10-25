/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/algorithms/LexMIB.h"
#include"../ground_truth_mibs.h"

/**
 * Test the LexMIB.cpp main algorithm
 */
int test_lexmib_lexmib_medium(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_delay_conditions.txt";
    Graph input_g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // import ground truth bicliques
    ground_truth_mibs GT;
    std::vector<std::vector<size_t>> mibs_true_raw = GT.mibs_true_raw("./test/ground_truth_medium.txt");

    // Create some random permutations of the input graph labels
    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13},
        {13,6,0,1,4,9,5,2,10,12,11,7,3,8},
        {2, 8, 3, 4, 1, 12, 0, 10, 9, 5, 6, 11, 7, 13},
        {11, 12, 5, 0, 4, 13, 6, 2, 8, 9, 1, 10, 3, 7},
        {9, 10, 11, 1, 8, 5, 6, 0, 3, 13, 12, 7, 2, 4},
        {3, 4, 0, 5, 10, 9, 8, 13, 6, 1, 11, 7, 12, 2},
        {6, 0, 12, 2, 11, 5, 9, 7, 8, 10, 3, 4, 13, 1},
        {4, 8, 1, 2, 13, 7, 5, 0, 6, 10, 3, 11, 9, 12},
        {12, 10, 8, 3, 4, 0, 11, 2, 13, 6, 7, 1, 5, 9},
        {2, 13, 0, 11, 4, 7, 3, 8, 12, 9, 6, 10, 5, 1},
        {2, 11, 4, 12, 9, 1, 13, 3, 10, 7, 6, 0, 8, 5},
        {6, 10, 0, 3, 1, 11, 12, 9, 5, 4, 7, 2, 13, 8}
    };

    // Iterate over each such permutation and run lexmib on the permuted graph
    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        // Create permuted graph, and create reverse label mapping
        Graph subgraph = input_g.subgraph(ordering);
        std::vector<size_t> reverse_ordering(ordering.size());
        for (size_t idx1=0; idx1<ordering.size(); idx1++) {
            reverse_ordering[ordering[idx1]] = idx1;
        }

        // Test basic graph information
        if (subgraph.get_num_edges() != input_g.get_num_edges()) {
            std::cout << "ERROR: Num edges should be " << input_g.get_num_edges()
            << ", is " << subgraph.get_num_edges() << std::endl;
            error = true;
        }
        if (subgraph.get_num_vertices() != input_g.get_num_vertices()) {
            std::cout << "ERROR: Num vertices should be " << input_g.get_num_vertices()
            << ", is " << subgraph.get_num_vertices() << std::endl;
            error = true;
        }

        // relabel groundtruth using new labeling
        auto mibs_true = convert_node_labels_vector(mibs_true_raw,
                                                      reverse_ordering);

        // Load the ground truth into a hashtable, each biclique as a string
        // mibs_map  -- to track how many times lexmib finds each mib
        // mibs_map_true -- holds just the ground truth
        std::unordered_map<std::string,size_t> mibs_map;
        for (size_t idx1 = 0; idx1<mibs_true.size(); idx1++) {
            auto this_mib = mibs_true[idx1];
            mibs_map[vector_to_string(this_mib)] = 0;
        }
        const auto mibs_map_true = mibs_map;

        auto mibs_computed = lexmib(subgraph);

        for (auto mib: mibs_computed){
            if (mibs_map.find(mib.to_string()) != mibs_map.end()){
                mibs_map[mib.to_string()] ++;
            }
            else {
                mibs_map[mib.to_string()] = 1;
            }
        }

        // Check we found correct number of mib
        if (mibs_true.size() != mibs_computed.size()) {
            std::cout << "ERROR: lexmib found wrong number of mibs: ";
            std::cout << mibs_computed.size() << " instead of correct number ";
            std::cout << mibs_true.size() << std::endl;
            error=true;
        }
        for (auto true_mib: mibs_true) {
            std::string true_mib_string = vector_to_string(true_mib);
            if (mibs_map[true_mib_string] != 1) {
                std::cout << "ERROR: lexmib wrong about true mib; " << true_mib_string << std::endl;
                std::cout << "number of times found: " << mibs_map[true_mib_string] << std::endl;
                error=true;
            }
        }
        for (auto iter: mibs_map) {
            if ( mibs_map_true.find(iter.first) == mibs_map_true.end() ) {
                std::cout << "ERROR: lexmib found wrong mib: " << iter.first << std::endl;
                error=true;
            }
        }

        LexMIBResults lexmibresults;
        lexmibresults.count_only_mode = true;
        lexmib(lexmibresults, subgraph);

        size_t computed_number_mibs = lexmibresults.total_num_mibs;

        // Check we found correct number of mib
        if (mibs_true.size() != computed_number_mibs) {
            std::cout << "ERROR: lexmib count_only_mode found wrong number of mibs: ";
            std::cout << computed_number_mibs << " instead of correct number ";
            std::cout << mibs_true.size() << std::endl;
            error=true;
        }

    }

    return error;

}
