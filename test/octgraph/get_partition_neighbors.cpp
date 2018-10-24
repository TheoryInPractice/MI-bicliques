/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/graph/OctGraph.h"


int test_octgraph_get_partition_neighbors(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Set up octgraph
    OrderedVertexSet oct_partition((std::vector<size_t>){2,4});
    OrderedVertexSet left_partition((std::vector<size_t>){0,3,6});
    OrderedVertexSet right_partition((std::vector<size_t>){1,5});

    OctGraph og(g,oct_partition,left_partition,right_partition);

    // For each vertex, compare computed partition-neighborhood
    // with its true partition neighborhood.
    std::vector<OrderedVertexSet> all_partitions = {oct_partition, left_partition, right_partition};
    for (auto idx : oct_partition) {

        // partition = oct
        OrderedVertexSet true_pt_neighb = oct_partition.set_intersection(og.get_neighbors(idx));

        OrderedVertexSet computed_pt_neighb = og.get_oct_neighbors(idx);

        if (true_pt_neighb != computed_pt_neighb) {
            std::cout << "OctGraph oct neighb wrong at vertex: " << idx << std::endl;
            error = true;
        }

        // partition = left
        true_pt_neighb = left_partition.set_intersection(og.get_neighbors(idx));

        computed_pt_neighb = og.get_left_neighbors(idx);

        if (true_pt_neighb != computed_pt_neighb) {
            std::cout << "OctGraph left neighb wrong at vertex: " << idx << std::endl;
            error = true;
        }

        // partition = right
        true_pt_neighb = right_partition.set_intersection(og.get_neighbors(idx));

        computed_pt_neighb = og.get_right_neighbors(idx);

        if (true_pt_neighb != computed_pt_neighb) {
            std::cout << "OctGraph right neighb wrong at vertex: " << idx << std::endl;
            error = true;
        }

    }

    return error;

}
