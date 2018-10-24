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
int test_searchtreebagplus_get_biclique(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_bipartite.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Set up octgraph
    OrderedVertexSet oct_partition((std::vector<size_t>){});
    OrderedVertexSet left_partition((std::vector<size_t>){0,4});
    OrderedVertexSet right_partition((std::vector<size_t>){1,2,3});

    OctGraph og(g,oct_partition,left_partition,right_partition);
    auto graph_ptr = std::make_shared<Graph>(og);


    // Now test constructor with iterative sets
    std::vector<size_t> completely_connected_in = {1,2,3};
    std::vector<size_t> iterative_wait = {4};
    std::vector<size_t> iterative_in = {0};
    std::vector<size_t> iterative_past = {};
    size_t expanded_vertex = 0;

    SearchTreeBagPlus test_bag(graph_ptr,
                               completely_connected_in,
                               iterative_wait,
                               iterative_in,
                               iterative_past,
                               expanded_vertex);

    // Check biclique
    BicliqueLite biclique_true(iterative_in, completely_connected_in);
    BicliqueLite biclique_computed = test_bag.get_biclique();
    if ( biclique_computed != biclique_true ) {
        std::cout << "SearchTreeBagPlus should have biclique ";
        std::cout << biclique_true.to_string() << std::endl;
        std::cout << "Not " << biclique_computed.to_string();
        std::cout << std::endl;
        error = true;
    }


    return error;

}
