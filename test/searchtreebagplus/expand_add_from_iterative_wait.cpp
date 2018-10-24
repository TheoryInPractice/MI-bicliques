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
#include"../../src/graph/SearchTreeBagPlus.h"

/**
 * Test SearchTreeBagPlus::expand, termination condition in which the
 * expanded node yields a biclique that can have something added from iter wait.
 */
int test_searchtreebagplus_expand_add_from_iterative_wait(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_delay_conditions.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);
    /* We use the following hard-coded map between vertex string labels
    * and vertex size_t labels
    * A 0
    * B 3
    * C 7
    * D 9
    * E 10
    * F 1
    * 1 4
    * 2 5
    * 3 2
    * 4 6
    * 5 11
    * 6 12
    * 7 13
    * 8 8
    */

    // Set up octgraph
    OrderedVertexSet oct_partition((std::vector<size_t>){0,1,3,7,9,10});
    OrderedVertexSet left_partition((std::vector<size_t>){8,11,12,13});
    OrderedVertexSet right_partition((std::vector<size_t>){2,4,5,6});

    // Make OctGraph with those partitions
    OctGraph og(g,oct_partition,left_partition,right_partition);
    std::shared_ptr<Graph> graph_ptr = std::make_shared<Graph>(og);

    // Bag for expand() to terminate with fail
    std::vector<size_t> completely_connected_in( {2,4} );
    std::vector<size_t> iterative_wait( {9,10} );
    std::vector<size_t> iterative_in( {3,7} );
    std::vector<size_t> iterative_past( {} );

    size_t expanded_vertex = 10;

    SearchTreeBagPlus test_bag(
                               graph_ptr,
                               completely_connected_in,
                               iterative_wait,
                               iterative_in,
                               iterative_past,
                               expanded_vertex
                              );

    bool expand_result = test_bag.expand(expanded_vertex);

    // Check for successful expand
    if (expand_result) {
        std::cout << "SearchTreeBagPlus expand should have failed.";
        std::cout << std::endl;
        error = true;
    }

    return error;

}
