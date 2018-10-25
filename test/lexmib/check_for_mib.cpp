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
#include"../../src/graph/OrderedVector.h"
#include "../../src/algorithms/LexMIB.h"


/**
 * Test LexMIB::check_for_mib
 */
int test_lexmib_check_for_mib(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Hard code starting points
    OrderedVertexSet set1((std::vector<size_t>) {0,3});
    OrderedVertexSet set2((std::vector<size_t>) {1});

    // Hardcode solutions
    // There are 7 iterations (one for each graph vertex).
    // For each iteration, there are two swaps (checking both (x,y) and (y,x)).
    // Each swap produces potentially several bicliques, hence a vector of strings.
    // Each string is a single biclique.
    std::vector<size_t> emptyset1,emptyset2;
    BicliqueLite empty_mib(emptyset1,emptyset2);
    std::string empty_string("");
    std::vector<std::vector<std::vector<std::string>>> mibs_true_raw = {
        {{""}, {""}},
        {{""}, {""}},
        {{std::string("1,2,3,4,")}, {std::string("0,2,4,")}},
        {{""}, {""}},
        {{}, {}},
        {{std::string("1,3,5,"), std::string("2,4,5,"), std::string("3,5,6,")}, {std::string("1,3,5,")}},
        {{std::string("2,3,4,6,"), std::string("3,5,6,")}, {std::string("2,3,4,6,"), std::string("3,5,6,")}}
    };

    // For each vertex in the graph...
    for (size_t vertex = 0; vertex < mibs_true_raw.size(); vertex++) {

        // Run on both (x,y) and (y,x)
        std::vector<OrderedVertexSet*> pair = {&set1, &set2};
        for (auto which_pair_order : {0,1}) {

            // Get the right ordered pair of sets
            OrderedVertexSet * x = pair[which_pair_order];
            OrderedVertexSet * y = pair[1-which_pair_order];

            OrderedVertexSet combined_x_y = x->set_union(*y);

            // skip vertices in x, y
            if (combined_x_y.has_vertex(vertex)) {
                continue;
            }
            OrderedVector xv = OrderedVector(x->get_vertices());
            OrderedVector yv = OrderedVector(y->get_vertices());
            // Run actual function
            BicliqueArchive mib_archive;

            check_for_mib(g, xv, yv, vertex, mib_archive);

            // Make hashtable of strings for ground truth bicliques for this setting
            std::vector<std::string> this_mibs_true = mibs_true_raw[vertex][which_pair_order];
            std::unordered_map<std::string,bool> mibs_true_map;
            for (std::string mib: this_mibs_true) {
                mibs_true_map[mib] = true;
            }

            // Check that correct number of MIBs computed
            if (mib_archive.heap_size() != mibs_true_map.size()){
                std::cout << "ERROR:  testing case: " << vertex << ", swap " << which_pair_order << ";\n\t found " << mib_archive.heap_size();
                std::cout << " mibs instead of correct number " << mibs_true_map.size() << std::endl;
                error = true;
            }

            // For each mib computed, pop it off the heap and check if it's true
            while  (mib_archive.heap_size() > 0) {
                auto mib_computed = mib_archive.top();
                mib_archive.pop();

                if (mibs_true_map.find(mib_computed.to_string()) == mibs_true_map.end()){
                    std::cout << "ERROR: Testing case: " << vertex << ", swap " << which_pair_order << std::endl;
                    std::cout << "\tComputed MIB doesn't appear in groundtruth: " << mib_computed.to_string() << std::endl;
                    error = true;
                }
            }
        }
    }

    return error;

}
