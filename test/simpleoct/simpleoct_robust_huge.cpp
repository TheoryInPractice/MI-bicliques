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
 * Test SimpleOCT::simpleoct_robust_huge -- performs same check as
 * simpleoct_robust, but on a larger dataset.
 */
int test_simpleoct_simpleoct_robust_huge(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_mcb_huge.txt";
    const Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Test the code on different permutations of the same graph
    const std::vector<std::vector<size_t>> orderings = {
        {36, 4, 7, 25, 32, 39, 28, 17, 38, 35, 19, 11, 22, 12, 16, 33, 2, 34, 15, 37, 29, 3, 10, 24, 21, 23, 14, 9, 0, 30, 5, 8, 31, 27, 13, 26, 6, 1, 20, 18},
        {22, 11, 27, 10, 31, 36, 14, 9, 26, 28, 33, 21, 18, 17, 19, 0, 6, 16, 37, 23, 4, 30, 7, 3, 8, 35, 20, 13, 12, 38, 39, 24, 25, 1, 34, 2, 5, 32, 15, 29},
        {4, 12, 15, 21, 18, 23, 9, 33, 37, 3, 35, 29, 27, 38, 8, 2, 34, 13, 30, 36, 39, 19, 0, 22, 28, 17, 1, 24, 5, 26, 32, 10, 7, 31, 6, 11, 20, 16, 25, 14},
        {32, 5, 7, 27, 21, 6, 39, 33, 10, 11, 36, 31, 28, 23, 25, 16, 24, 4, 20, 34, 2, 1, 0, 38, 19, 9, 30, 15, 29, 8, 17, 14, 22, 35, 37, 3, 26, 12, 13, 18},
        {4, 22, 11, 13, 15, 36, 31, 32, 17, 3, 35, 26, 12, 18, 7, 28, 19, 21, 20, 1, 34, 23, 5, 38, 29, 16, 30, 24, 10, 0, 33, 25, 6, 8, 2, 39, 14, 9, 37, 27},
        {7, 28, 6, 38, 18, 9, 31, 25, 12, 26, 29, 15, 11, 23, 34, 0, 33, 35, 10, 22, 37, 1, 21, 20, 3, 14, 36, 13, 32, 5, 16, 4, 24, 27, 2, 8, 39, 19, 17, 30},
        {19, 32, 38, 25, 13, 0, 26, 21, 17, 11, 20, 8, 5, 33, 9, 2, 7, 1, 34, 29, 30, 3, 39, 28, 6, 24, 23, 36, 15, 14, 18, 22, 31, 37, 12, 35, 10, 16, 4, 27},
        {2, 16, 17, 26, 25, 11, 10, 19, 8, 20, 38, 28, 24, 15, 37, 6, 5, 32, 39, 9, 30, 34, 1, 23, 13, 27, 21, 33, 14, 31, 35, 18, 36, 12, 3, 4, 0, 29, 7, 22},
        {12, 9, 23, 7, 10, 31, 38, 29, 32, 39, 24, 19, 33, 14, 25, 35, 6, 16, 36, 28, 21, 37, 18, 2, 0, 4, 5, 34, 1, 22, 20, 13, 27, 26, 30, 15, 8, 17, 3, 11},
        {13, 12, 21, 15, 29, 3, 33, 30, 2, 26, 20, 1, 18, 16, 8, 36, 24, 23, 5, 38, 32, 22, 34, 39, 4, 6, 0, 10, 14, 27, 11, 28, 9, 7, 37, 19, 25, 35, 31, 17}
    };

    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        Graph use_g = g.subgraph(ordering);
        OrderedVertexSet oct_computed, left_computed, right_computed;

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

    }

    return error;

}
