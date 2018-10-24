/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


#include <iostream>
#include <vector>
#include "../../src/graph/OctGraph.h"
#include "../../src/graph/SearchTreeBagPlus.h"

/**
 * Test that the SearchTreeBagPlus constructor works.
 */
int test_searchtreebagplus_all_functions(int argc, char **argv) {

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
    auto graph_ptr = std::make_shared<Graph>(og);

    size_t expanded_vertex = 73;  // arbitrary value for testing
    SearchTreeBagPlus test_bag(expanded_vertex);

    // Check expanded vertex
    if ( test_bag.get_expanded_vertex() != expanded_vertex ) {
        std::cout << "Empty SearchTreeBagPlus should have expanded_vertex = ";
        std::cout << expanded_vertex << ", not " << test_bag.get_expanded_vertex();
        std::cout << std::endl;
        error = true;
    }

    // Now test constructor with iterative sets
    std::vector<size_t> completely_connected_in = {9,10,11,12};
    std::vector<size_t> iterative_wait = {6,7,8};
    std::vector<size_t> iterative_in = {3,4,5};
    std::vector<size_t> iterative_past = {0,1,2};
    expanded_vertex = 6;

    SearchTreeBagPlus test_bag2(graph_ptr,
                                completely_connected_in,
                                iterative_wait,
                                iterative_in,
                                iterative_past,
                                expanded_vertex);

    // Check iterative sets inside searchtreebag
    if (test_bag2.get_iterative_wait() != iterative_wait) {
        std::cout << "SearchTreeBagPlus get_iterative_wait should return ";
        std::cout << vector_to_string(iterative_wait) << std::endl;
        std::cout << "not " << vector_to_string(test_bag2.get_iterative_wait());
        std::cout << std::endl;
        error = true;
    }
    // Check expanded vertex
    if ( test_bag2.get_expanded_vertex() != expanded_vertex ) {
        std::cout << "SearchTreeBagPlus should have expanded_vertex = ";
        std::cout << expanded_vertex << ", not " << test_bag.get_expanded_vertex();
        std::cout << std::endl;
        error = true;
    }


    return error;

}
