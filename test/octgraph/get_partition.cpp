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


int test_octgraph_get_partition(int argc, char ** argv) {

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

    // Check oct partitions are correct
    if (og.get_oct() != oct_partition) {
        std::cout << "OctGraph oct partition incorrect." << std::endl;
        error = true;
    }
    if (og.get_left() != left_partition) {
        std::cout << "OctGraph left partition incorrect." << std::endl;
        error = true;
    }
    if (og.get_right() != right_partition) {
        std::cout << "OctGraph right partition incorrect." << std::endl;
        error = true;
    }

    return error;

}
