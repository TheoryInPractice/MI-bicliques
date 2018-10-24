/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/algorithms/OCTMIB.h"
#include"../ground_truth_mibs.h"

/**
 * Test the OCTMIB.cpp algorithm
 */
int test_octmib_octmib_multi_ccs(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_multi_ccs.txt";
    Graph input_g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // import ground truth bicliques
    ground_truth_mibs GT;
    std::vector<std::vector<size_t>> mibs_true_raw = GT.mibs_true_raw("./test/ground_truth_multi_ccs.txt");

    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4,5,6,7,8,9},
        {6,1,2,3,4,5,0,7,8,9},
        {6,1,5,2,4,3,0,7,8,9},
        {0,6,2,5,3,4,1,7,8,9}
    };

    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        std::cout << "\n\n\nOrdering " << idx + 1 << " / " << orderings.size() << std::endl;

        Graph subgraph = input_g.subgraph(ordering);
        std::vector<size_t> reverse_ordering(ordering.size());
        for (size_t idx1=0; idx1<ordering.size(); idx1++) reverse_ordering[ordering[idx1]] = idx1;

        // Test basic graph information
        if (subgraph.get_num_edges() != input_g.get_num_edges()) {
            std::cout << "Num edges should be " << input_g.get_num_edges() << ", is " << subgraph.get_num_edges() << std::endl;
            error = true;
        }
        if (subgraph.get_num_vertices() != input_g.get_num_vertices()) {
            std::cout << "Num vertices should be " << input_g.get_num_vertices() << ", is " << subgraph.get_num_vertices() << std::endl;
            error = true;
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
                std::cout << "ERROR: OCTMIB found thing not in ground truth: " << iter.first << std::endl;
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
