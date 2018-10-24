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
#include"../../src/graph/OrderedVertexSet.h"
#include"../../src/algorithms/SimpleOCT.h"

/**
 * Test SimpleOCT::simpleoct -- test that SimpleOCT works as intended on a
 * small graph.
 */
int test_simpleoct_simpleoct(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Set up ground truth
    OrderedVertexSet oct_true((std::vector<size_t>){2,5});
    OrderedVertexSet left_true((std::vector<size_t>){0,3,6});
    OrderedVertexSet right_true((std::vector<size_t>){1,4});

    OrderedVertexSet oct_computed, left_computed, right_computed;

    simpleoct(g, oct_computed, left_computed, right_computed);

    std::vector<OrderedVertexSet> sets_true = {oct_true, left_true, right_true};
    std::vector<OrderedVertexSet> sets_computed = {oct_computed, left_computed, right_computed};

    for (size_t idx = 0; idx < sets_true.size(); idx++) {

        auto this_true = sets_true[idx];
        auto this_computed = sets_computed[idx];

        if (this_true != this_computed) {
            std::cout << "SimpleOCT output incorrect. Should be: ";
            for (auto v : this_true) std::cout << v << " ";
            std::cout << std::endl;

            std::cout << "but computed as: ";
            for (auto v : this_computed) std::cout << v << " ";
            std::cout << std::endl;

            error = true;
        }

    }

    return error;

}
