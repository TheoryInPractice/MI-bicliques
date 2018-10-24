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
#include"../../src/algorithms/AllMaximalIndependentSets.h"

/**
 * Test AllMaximalIndependentSets::get_all_mis
 */
int test_allmaximalis_get_all_mis(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Hard code all independent sets
    std::unordered_map<std::string,size_t> MIS_true = {
        {"0,3,6,", 0},
        {"0,4,", 0},
        {"0,5,", 0},
        {"1,4,", 0},
        {"1,5,", 0},
        {"1,6,", 0},
        {"2,3,6,", 0},
        {"2,5,", 0},
    };

    auto list_of_MIS = get_all_mis(g);

    // Check every MIS found is actually in the true set
    for (auto mis : list_of_MIS) {
        // convert MIS to string
        std::stringstream mis_s;
        for (auto v : mis) mis_s << v << ",";

        std::string this_mis = mis_s.str();
        if (MIS_true.find(this_mis) == MIS_true.end()){
            std::cout << "get_all_mis is missing independent set " << this_mis <<std::endl;
            error=true;
        }
        else {
            MIS_true[this_mis] ++;
        }
    }

    // Check we found correct number of MIS
    if (MIS_true.size() != list_of_MIS.size()) {
        std::cout << "get_all_mis found wrong number of MISs: " << list_of_MIS.size() << " instead of correct number " << MIS_true.size() << std::endl;
        error=true;
    }

    // Check how many times each MIS was found (should be 1 each)
    for (auto const & iter : MIS_true) {
        std::string this_mis = iter.first;
        size_t this_count = iter.second;

        if (this_count != 1) {
            std::cout << "get_all_mis found a MIS " << this_count << "times: " << this_mis << std::endl;
            error=true;
        }

    }

    return error;

}
