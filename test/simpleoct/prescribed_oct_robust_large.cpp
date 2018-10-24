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
#include"../../src/graph/Graph.h"
#include"../../src/graph/OrderedVertexSet.h"
#include"../../src/algorithms/SimpleOCT.h"

/**
 * Test SimpleOCT::prescribed_oct_robust_large -- tests that SimpleOCT works
 * as intended when the input includes a prescribed OCT set. Test set is large
 * and "robust" means the code is tested over multiple permutations of the
 * graph's nodes.
 */
int test_simpleoct_prescribed_oct_robust_large(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_large.txt";
    const Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Test the code on different permutations of the same graph
    const std::vector<std::vector<size_t>> orderings = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
        {16, 11, 3, 8, 6, 5, 17, 20, 14, 13, 15, 23, 2, 7, 22, 25, 12, 26, 4, 10, 29, 28, 19, 27, 18, 9, 24, 0, 21, 1},
        {28, 9, 13, 14, 29, 26, 4, 27, 25, 18, 10, 0, 1, 8, 22, 2, 12, 5, 20, 11, 6, 16, 15, 23, 3, 7, 17, 21, 19, 24},
        {5, 0, 1, 21, 4, 13, 9, 8, 3, 25, 10, 2, 22, 7, 19, 24, 27, 28, 29, 23, 16, 6, 11, 14, 20, 12, 17, 15, 18, 26},
        {14, 29, 3, 23, 10, 25, 1, 18, 16, 2, 17, 5, 9, 8, 11, 20, 24, 13, 27, 26, 7, 12, 19, 4, 15, 21, 0, 28, 6, 22},
        {27, 16, 13, 22, 28, 11, 5, 20, 3, 0, 6, 25, 26, 10, 29, 12, 24, 1, 14, 2, 23, 21, 9, 18, 4, 17, 7, 8, 15, 19},
        {14, 13, 10, 0, 4, 8, 19, 20, 18, 7, 29, 6, 1, 28, 5, 26, 27, 17, 15, 25, 23, 12, 11, 2, 22, 9, 24, 3, 16, 21},
        {14, 11, 9, 28, 8, 16, 7, 25, 26, 24, 20, 4, 17, 29, 22, 13, 15, 27, 2, 3, 19, 23, 10, 0, 12, 1, 18, 5, 21, 6},
        {20, 0, 29, 3, 25, 19, 11, 18, 15, 22, 24, 4, 27, 21, 12, 6, 5, 16, 7, 23, 28, 17, 1, 9, 26, 14, 8, 13, 2, 10},
        {20, 29, 4, 1, 15, 11, 18, 26, 5, 12, 0, 10, 23, 19, 7, 3, 2, 8, 24, 13, 28, 6, 25, 17, 14, 16, 21, 9, 22, 27},
        {10, 8, 1, 16, 26, 4, 6, 20, 22, 7, 24, 13, 17, 15, 3, 9, 19, 11, 14, 18, 5, 12, 21, 23, 2, 29, 28, 25, 0, 27},
        {20, 4, 1, 12, 23, 19, 3, 6, 0, 21, 18, 28, 27, 2, 13, 29, 10, 8, 16, 26, 25, 9, 14, 7, 5, 24, 15, 22, 17, 11},
        {1, 6, 14, 3, 11, 10, 20, 19, 17, 0, 2, 23, 4, 27, 21, 18, 24, 22, 7, 26, 9, 5, 12, 8, 13, 25, 15, 29, 28, 16},
        {26, 10, 0, 1, 15, 12, 29, 7, 25, 5, 21, 11, 18, 27, 3, 9, 23, 22, 17, 19, 4, 14, 13, 8, 28, 16, 6, 2, 20, 24},
        {25, 8, 3, 16, 13, 15, 9, 20, 24, 21, 18, 23, 1, 10, 22, 2, 14, 6, 4, 7, 17, 19, 0, 28, 27, 11, 26, 29, 5, 12},
        {1, 23, 16, 3, 15, 25, 8, 18, 20, 21, 10, 2, 7, 28, 11, 27, 22, 13, 19, 14, 29, 6, 9, 4, 26, 12, 0, 5, 17, 24}
    };

    const std::vector<std::vector<size_t>> oct_sets_raw = {
        {7,8,9,11,13,14,16,18,19,20,21,22,23,24,26,28},
        {1,2,3,5,7,9,13,14,15,17,18,20,21,22,23,24,26,27},
        {1,6,7,8,9,10,12,15,17,18,19,20,21,22,23,24,29},
        {0,2,6,7,8,10,11,12,13,15,16,17,18,19,21,25,27,28},
        {0,3,5,7,9,11,13,14,16,18,19,21,22,23,25,26,27,28}
    };

    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        Graph use_g = g.subgraph(ordering);

        std::vector<size_t> reverse_ordering(ordering.size());
        for (size_t idx1=0; idx1<ordering.size(); idx1++) {
            reverse_ordering[ordering[idx1]] = idx1;
        }

        for (auto oct_set_raw: oct_sets_raw) {

            OrderedVertexSet left_computed, right_computed;

            OrderedVertexSet oct_set(oct_set_raw);
            OrderedVertexSet oct_computed = convert_node_labels_OVS(oct_set, reverse_ordering);

            clock_t begin = std::clock();
            simpleoct(use_g, oct_computed, left_computed, right_computed);
            clock_t end = std::clock();
            double elapsed_time = double(end - begin) / CLOCKS_PER_SEC;

            // Check the coloring is valid
            for (auto v: left_computed) {
                OrderedVertexSet this_left = left_computed.set_minus(OrderedVertexSet((std::vector<size_t>){v}));
                if (!use_g.is_completely_independent_from(v, this_left)) {
                    std::cout << "SimpleOCT output incorrect on left partition." << std::endl;
                    std::cout << "Vertex " << v << " has neighbor in same partition." << std::endl;
                    error = true;
                }
            }

            for (auto v: right_computed) {
                OrderedVertexSet this_right = right_computed.set_minus(OrderedVertexSet((std::vector<size_t>){v}));
                if (!use_g.is_completely_independent_from(v, this_right)) {
                    std::cout << "SimpleOCT output incorrect on right partition." << std::endl;
                    std::cout << "Vertex " << v << " has neighbor in same partition." << std::endl;
                    error = true;
                }
            }

        } // end FOR loop over different prescribed OCT sets

    } // end FOR loop over different permutations of graph.

    return error;

}
