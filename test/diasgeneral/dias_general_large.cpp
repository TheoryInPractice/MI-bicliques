/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/algorithms/DiasGeneral.h"
#include"../ground_truth_mibs.h"

/**
 * Test the DiasGeneral.cpp main algorithm
 */
int test_diasgeneral_dias_general_large(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_large.txt";
    Graph input_g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // import ground truth bicliques
    ground_truth_mibs GT;
    std::vector<std::vector<size_t>> mibs_true_raw = GT.mibs_true_raw("./test/ground_truth_large.txt");

    // Create some random permutations of the input graph labels
    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29},
        {16,11,3,8,6,5,17,20,14,13,15,23,2,7,22,25,12,26,4,10,29,28,19,27,18,9,24,0,21,1},
        {25,8,3,16,13,15,9,20,24,21,18,23,1,10,22,2,14,6,4,7,17,19,0,28,27,11,26,29,5,12},
        {1,23,16,3,15,25,8,18,20,21,10,2,7,28,11,27,22,13,19,14,29,6,9,4,26,12,0,5,17,24}
    };

    // Iterate over each such permutation and run dias_general on the permuted graph
    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        // Create permuted graph, and create reverse label mapping
        Graph subgraph = input_g.subgraph(ordering);
        std::vector<size_t> reverse_ordering(ordering.size());
        for (size_t idx1=0; idx1<ordering.size(); idx1++) {
            reverse_ordering[ordering[idx1]] = idx1;
        }

        // relabel groundtruth using new labeling
        auto mibs_true = convert_node_labels_vector(mibs_true_raw,
                                                      reverse_ordering);

        // Load the ground truth into a hashtable, each biclique as a string
        // mibs_map  -- to track how many times dias_general finds each mib
        // mibs_map_true -- holds just the ground truth
        std::unordered_map<std::string,size_t> mibs_map;
        for (size_t idx1 = 0; idx1<mibs_true.size(); idx1++) {
            auto this_mib = mibs_true[idx1];
            mibs_map[vector_to_string(this_mib)] = 0;
        }
        const auto mibs_map_true = mibs_map;

        auto mibs_computed = dias_general(subgraph);

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
            std::cout << "ERROR: dias_general found wrong number of mibs: ";
            std::cout << mibs_computed.size() << " instead of correct number ";
            std::cout << mibs_true.size() << std::endl;
            error=true;
        }
        for (auto true_mib: mibs_true) {
            std::string true_mib_string = vector_to_string(true_mib);
            if (mibs_map[true_mib_string] != 1) {
                std::cout << "ERROR: dias_general wrong about true mib; " << true_mib_string << std::endl;
                std::cout << "number of times found: " << mibs_map[true_mib_string] << std::endl;
                error=true;
            }
        }
        for (auto iter: mibs_map) {
            if ( mibs_map_true.find(iter.first) == mibs_map_true.end() ) {
                std::cout << "ERROR: dias_general found wrong mib: " << iter.first << std::endl;
                error=true;
            }
        }

        DiasResults diasresults;
        diasresults.count_only_mode = true;
        dias_general(diasresults, subgraph);

        size_t computed_number_mibs = diasresults.total_num_mibs;

        // Check we found correct number of mib
        if (mibs_true.size() != computed_number_mibs) {
            std::cout << "ERROR: dias_general count_only_mode found wrong number of mibs: ";
            std::cout << computed_number_mibs << " instead of correct number ";
            std::cout << mibs_true.size() << std::endl;
            error=true;
        }

    }

    return error;

}
