/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include <iostream>
#include <vector>
#include "../../src/graph/OrderedVertexSet.h"  // for relabling
#include "../../src/graph/Graph.h"
#include "../../src/algorithms/SimpleCCs.h"

/**
 * Test SimpleCCs::simpleccs
 */
int test_simpleccs_simpleccs_robust(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_multi_ccs.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Ground truth hard coded
    const std::vector<std::vector<size_t>> ccs_true_raw = {
        (std::vector<size_t>) {0,1,2,3},
        (std::vector<size_t>) {4,5,6},
        (std::vector<size_t>) {9},
        (std::vector<size_t>) {7,8}
    };

    // Test the code on different permutations of the same graph
    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4,5,6,7,8,9},
        {6,1,2,3,4,5,0,9,8,7},
        {7,9,8,6,1,5,2,4,3,0},
        {0,7,6,2,8,5,9,3,4,1}
    };

    g.print_graph();

    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        Graph use_g = g.subgraph(ordering);

        // relabel ground truth using this ordering
        std::vector<size_t> reverse_ordering(ordering.size());
        for (size_t idx1=0; idx1<ordering.size(); idx1++) {
            reverse_ordering[ordering[idx1]] = idx1;
        }

        auto ccs_true = convert_node_labels_vector(ccs_true_raw, reverse_ordering);

        std::unordered_map<std::string,size_t> ccs_map;
        for (size_t idx1 = 0; idx1<ccs_true.size(); idx1++) {
            auto this_cc = ccs_true[idx1];
            ccs_map[vector_to_string(this_cc)] = 0;
        }
        const auto ccs_map_true = ccs_map;

        // Call function we're testing
        auto ccs_computed = simpleccs(use_g);

        // Convert computed ccs to strings for verification
        for (auto this_cc: ccs_computed){
      	    std::string this_cc_string = vector_to_string(this_cc);
            if (ccs_map.find(this_cc_string) != ccs_map.end()){
                ccs_map[this_cc_string] ++;
            }
            else {
                ccs_map[this_cc_string] = 1;
            }
        }

        // Check we found correct number of mib
        if (ccs_true.size() != ccs_computed.size()) {
            std::cout << "ERROR: simpleccs found wrong number of ccs: ";
            std::cout << ccs_computed.size() << " instead of correct number ";
            std::cout << ccs_true.size() << std::endl;
            error=true;
        }
        for (auto true_cc: ccs_true) {
            std::string true_cc_string = vector_to_string(true_cc);
            if (ccs_map[true_cc_string] != 1) {
                std::cout << "ERROR: simpleccs wrong about true cc; " << true_cc_string << std::endl;
                std::cout << "number of times found: " << ccs_map[true_cc_string] << std::endl;
                error=true;
            }
        }
        for (auto iter: ccs_map) {
            if ( ccs_map_true.find(iter.first) == ccs_map_true.end() ) {
                std::cout << "ERROR: simpleccs found wrong cc: " << iter.first << std::endl;
                error=true;
            }
        }

    }

    return error;

}
