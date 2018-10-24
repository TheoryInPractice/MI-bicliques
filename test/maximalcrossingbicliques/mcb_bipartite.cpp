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
#include"../../src/algorithms/MaximalCrossingBicliquesBipartite.h"

/**
 * Test MaximalCrossingBicliques::maximal_crossing_bicliques
 */
int test_maximalcrossingbicliques_mcb_bipartite(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_bipartite.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Hard code decomposition
    OrderedVertexSet left_raw((std::vector<size_t>) {0,4});
    OrderedVertexSet right_raw((std::vector<size_t>) {1,2,3});

    // Hard code maximal bicliques crossing from one set to other.
    std::vector<OrderedVertexSet> mcb_true_raw{
                            OrderedVertexSet((std::vector<size_t>){0,1,2,3}),
                            OrderedVertexSet((std::vector<size_t>){0,2,3,4})
    };

    const std::vector<std::vector<size_t>> orderings = {
        {0,1,2,3,4},
        {1,2,3,4,0},
        {1,2,4,3,0},
        {0,2,3,4,1}
    };

    for (size_t idx = 0; idx < orderings.size(); idx++) {

        std::cout << "ORDERING: " << idx << std::endl;

        const auto ordering = orderings[idx];

        Graph subgraph = g.subgraph(ordering);
        std::vector<size_t> reverse_ordering(ordering.size());
        for (size_t idx1=0; idx1<ordering.size(); idx1++) {
            reverse_ordering[ordering[idx1]] = idx1;
        }

        std::cout << "after subgraph " << std::endl;

        // relabel groundtruth using new labeling
        std::vector<OrderedVertexSet> mcb_true = convert_node_labels_OVS(mcb_true_raw, reverse_ordering);
        OrderedVertexSet left_set = convert_node_labels_OVS(left_raw, reverse_ordering);
        OrderedVertexSet right_set = convert_node_labels_OVS(right_raw, reverse_ordering);

        std::cout << "after relabel " << idx << std::endl;

        std::unordered_map<std::string,size_t> mcb_map;
        for (auto this_mcb: mcb_true) {
            mcb_map[this_mcb.to_string()] = 0;
        }
        const auto mcb_map_true = mcb_map;

        std::cout << "Before call to MCBB: " << std::endl;

        auto mcbs_computed = maximal_crossing_bicliques_bipartite(subgraph, left_set.get_vertices(), right_set.get_vertices());

        // Check every MCB found is actually in the true set
        for (auto mcb : mcbs_computed) {

            // convert mcb to string
            std::string this_mcb = mcb.to_string();

            std::cout << "in test: " << this_mcb << std::endl;

            if (mcb_map.find(this_mcb) == mcb_map.end()){
                std::cout << "Testing case: " << idx << " : ";
                std::cout << "maximal_crossing_bicliques_bipartite found non-MIB " << this_mcb <<std::endl;
                error=true;
            }
            else {
                mcb_map[this_mcb] ++;
            }
        }

        // Check we found correct number of MCB
        if (mcb_map.size() != mcbs_computed.size()) {
            std::cout << "Testing case: " << idx << " : ";
            std::cout << "maximal_crossing_bicliques_bipartite found wrong number of mcbs: " << mcbs_computed.size() << " instead of correct number " << mcb_true.size() << std::endl;
            error=true;
        }

        // Check how many times each MCB was found (should be 1 each)
        for (auto const & iter : mcb_map) {
            std::string this_mcb = iter.first;
            size_t this_count = iter.second;

            if (this_count != 1) {
                std::cout << "Testing case: " << idx << " : ";
                std::cout << "maximal_crossing_bicliques_bipartite found a mcb " << this_count << " times: " << this_mcb << std::endl;
                error=true;
            }

        }
    }

    return error;

}
