/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include <ctime> // for timing
#include<iostream>
#include<vector>
#include"../../src/algorithms/OCTMIB.h"
#include"../ground_truth_mibs.h"

/**
 * Test the OCTMIB.cpp algorithm
 */
int test_octmib_octmib_huge(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_mcb_huge.txt";
    Graph input_g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // import ground truth bicliques
    ground_truth_mibs GT;
    std::vector<std::vector<size_t>> mibs_true_raw = GT.mibs_true_raw("./test/ground_truth_huge.txt");


    // Create some random permutations of the input graph labels
    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39}
    };

    // Iterate over each such permutation and run OCTMIB on the permuted graph
    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        // Create permuted graph, and create reverse label mapping
        Graph subgraph = input_g.subgraph(ordering);
        std::vector<size_t> reverse_ordering(ordering.size());
        for (size_t idx1=0; idx1<ordering.size(); idx1++) {
            reverse_ordering[ordering[idx1]] = idx1;
        }

        // relabel groundtruth using new labeling
        auto mibs_true = convert_node_labels_vector(mibs_true_raw, reverse_ordering);

        std::unordered_map<std::string,size_t> mibs_map;
        for (size_t idx1 = 0; idx1<mibs_true.size(); idx1++) {
            auto this_mib = mibs_true[idx1];
            mibs_map[vector_to_string(this_mib)] = 0;
        }
        const auto mibs_map_true = mibs_map;

        auto mibs_computed = octmib(subgraph);

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
            std::cout << "ERROR: OCTMIB found wrong number of mibs: ";
            std::cout << mibs_computed.size() << " instead of correct number ";
            std::cout << mibs_true.size() << std::endl;
            error=true;
        }
        for (auto true_mib: mibs_true) {
            std::string true_mib_string = vector_to_string(true_mib);
            if (mibs_map[true_mib_string] != 1) {
                std::cout << "ERROR: OCTMIB wrong about true mib; " << true_mib_string << std::endl;
                std::cout << "number of times found: " << mibs_map[true_mib_string] << std::endl;
                error=true;
            }
        }
        for (auto iter: mibs_map) {
            if ( mibs_map_true.find(iter.first) == mibs_map_true.end() ) {
                std::cout << "ERROR: OCTMIB found wrong mib: " << iter.first << std::endl;
                error=true;
            }
        }

        OutputOptions resultsobject;
        resultsobject.count_only_mode = true;
        octmib(resultsobject, subgraph);

        size_t computed_number_mibs = resultsobject.total_num_mibs;

        // Check we found correct number of mib
        if (mibs_true.size() != computed_number_mibs) {
            std::cout << "ERROR: OCTMIB count_only_mode found wrong number of mibs: ";
            std::cout << computed_number_mibs << " instead of correct number ";
            std::cout << mibs_true.size() << std::endl;
            error=true;
        }
    }


    return error;

}
