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
 * Test the full LexMIB.cpp algorithm
 */
int test_lexmib_count_only_multi_ccs(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_multi_ccs.txt";
    Graph input_g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // import ground truth bicliques
    ground_truth_mibs GT;
    std::vector<std::vector<size_t>> mibs_true_raw = GT.mibs_true_raw("./test/ground_truth_multi_ccs.txt");

    const size_t GROUND_TRUTH_NUMBER_MIBS = mibs_true_raw.size();

    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4,5,6,7,8,9},
        {6,1,2,3,4,5,0,7,8,9},
        {6,1,5,2,4,3,0,7,8,9},
        {0,6,2,5,3,4,1,7,8,9},
        {1, 3, 6, 5, 2, 0, 4,7,8,9},
        {0, 3, 4, 5, 2, 6, 1,7,8,9},
        {1, 5, 2, 4, 3,7,8,9, 6, 0},
        {2, 3, 0, 6, 4,7,8,9, 5, 1},
        {1, 4,7,8,9, 3, 5, 6, 0, 2},
        {5, 0, 6,7,8,9, 4, 1, 3, 2},
        {1, 4, 6, 0, 3, 2, 5,7,8,9},
        {2, 4, 3, 6,7,8,9, 0, 5, 1},
        {0, 4, 5, 1, 3,7,8,9, 6, 2},
        {2,7,8,9, 5, 4, 6, 0, 3, 1}
    };

    for (size_t idx = 0; idx < orderings.size(); idx++) {

        const auto ordering = orderings[idx];

        Graph subgraph = input_g.subgraph(ordering);

        LexMIBResults lexmibresults;
        lexmibresults.count_only_mode = true;
        lexmib(lexmibresults, subgraph);

        size_t computed_number_mibs = lexmibresults.total_num_mibs;

        // Check we found correct number of mib
        if (GROUND_TRUTH_NUMBER_MIBS != computed_number_mibs) {
            std::cout << "ERROR: lexmib count_only_mode found wrong number of mibs: ";
            std::cout << computed_number_mibs << " instead of correct number ";
            std::cout << GROUND_TRUTH_NUMBER_MIBS << std::endl;
            error=true;
        }

    }

    return error;

}
