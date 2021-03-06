/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<sstream>
#include<vector>
#include"../../src/graph/Graph.h"
#include"../../src/graph/OrderedVertexSet.h"
#include"../../src/algorithms/MaximalCrossingBicliques.h"

/**
 * Test MaximalCrossingBicliques::maximal_crossing_bicliques
 */
int test_maximalcrossingbicliques_mcb_large(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_delay_conditions.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Hard code independent sets
    std::vector<OrderedVertexSet> set_of_ISs = {
        OrderedVertexSet ((std::vector<size_t>) {8,11,12}),
        OrderedVertexSet ((std::vector<size_t>) {0,3,7,9,10})
    };

    // Hard code maximal bicliques crossing from one set to other.
    std::vector<std::unordered_map<std::string,size_t>> set_of_mcb_true = {
        std::unordered_map<std::string,size_t> {
                                {"5,7,8,", 0},
                                {"4,5,11,12,", 0},
                                {"5,8,11,12,", 0}
        },
        std::unordered_map<std::string,size_t> {
                                {"0,1,2,9,", 0},
                                {"0,2,3,7,9,10,", 0},
                                {"1,2,4,5,6,9,", 0},
                                {"2,3,4,5,6,9,", 0},
                                {"2,3,4,7,9,10,", 0},
                                {"2,4,7,8,", 0},
        }
    };

    for (size_t idx = 0; idx < set_of_ISs.size(); idx++){

        auto ind_set = set_of_ISs[idx];

        auto mcb_true = set_of_mcb_true[idx];

        auto mcbs_computed = maximal_crossing_bicliques(g, ind_set);

        // Check every MIS found is actually in the true set
        for (auto mcb : mcbs_computed) {

            // convert mcb to string
            std::string this_mcb = mcb.to_string();

            if (mcb_true.find(this_mcb) == mcb_true.end()){
                std::cout << "Testing case: " << idx << " : ";
                std::cout << "maximal_crossing_bicliques is missing biclique " << this_mcb <<std::endl;
                error=true;
            }
            else {
                mcb_true[this_mcb] ++;
            }
        }

        // Check we found correct number of MCB
        if (mcb_true.size() != mcbs_computed.size()) {
            std::cout << "Testing case: " << idx << " : ";
            std::cout << "maximal_crossing_bicliques found wrong number of mcbs: " << mcbs_computed.size() << " instead of correct number " << mcb_true.size() << std::endl;
            error=true;
        }

        // Check how many times each MCB was found (should be 1 each)
        for (auto const & iter : mcb_true) {
            std::string this_mcb = iter.first;
            size_t this_count = iter.second;

            if (this_count != 1) {
                std::cout << "Testing case: " << idx << " : ";
                std::cout << "maximal_crossing_bicliques found a mcb " << this_count << " times: " << this_mcb << std::endl;
                error=true;
            }

        }
    }

    return error;

}
