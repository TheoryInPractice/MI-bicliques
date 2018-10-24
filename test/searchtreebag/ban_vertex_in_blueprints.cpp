/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


#include <iostream>
#include <vector>
#include "../../src/graph/SearchTreeBag.h"
#include "../../src/graph/BlueprintLite.h"
#include "../../src/graph/OctGraph.h"
#include "../../src/graph/Graph.h"


/**
 * Test that the SearchTreeBag constructor works.
 */
int test_searchtreebag_ban_vertex_in_blueprints(int argc, char **argv) {

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
    OrderedVertexSet left_partition((std::vector<size_t>){8,11,12});
    OrderedVertexSet right_partition((std::vector<size_t>){2,4,5,6,13});

    // Make OctGraph with those partitions
    OctGraph og(g,oct_partition,left_partition,right_partition);
    std::shared_ptr<Graph> graph_ptr = std::make_shared<Graph>(og);

    // Blueprint 0
    size_t biclique_blueprint_id = 0;
    std::vector<size_t> iterative_wait( {7,10} );
    OrderedVertexSet iterative_in( (std::vector<size_t>){3,9} );
    std::vector<size_t> iterative_past( {} );
    std::vector<size_t> completely_connected_in( {2,4} );
    std::vector<size_t> completely_connected_out( {} );
    std::vector<size_t> independent_from_in( {11,12,13} );
    std::vector<size_t> independent_from_out( {} );
    std::vector<size_t> oct_auxiliary_independent_set( {1} );

    BlueprintLite test_blueprint(
                                 biclique_blueprint_id,
                                 graph_ptr,
                                 std::make_shared<std::vector<size_t>>(iterative_wait),
                                 std::make_shared<OrderedVertexSet>(iterative_in),
                                 std::make_shared<std::vector<size_t>>(iterative_past),
                                 completely_connected_in,
                                 completely_connected_out,
                                 independent_from_in,
                                 independent_from_out,
                                 oct_auxiliary_independent_set
                                );

    // Blueprint 1
    biclique_blueprint_id = 1;
    iterative_wait = std::vector<size_t> ( {10} );
    iterative_in = OrderedVertexSet ( (std::vector<size_t>){3,7,9} );
    iterative_past = std::vector<size_t> ( {} );
    completely_connected_in = std::vector<size_t> ( {2,4} );
    completely_connected_out = std::vector<size_t> ( {} );
    independent_from_in = std::vector<size_t> ( {11,12,13} );
    independent_from_out = std::vector<size_t> ( {} );
    oct_auxiliary_independent_set = std::vector<size_t> ( {1} );

    BlueprintLite test_blueprint2(
                                 biclique_blueprint_id,
                                 graph_ptr,
                                 std::make_shared<std::vector<size_t>>(iterative_wait),
                                 std::make_shared<OrderedVertexSet>(iterative_in),
                                 std::make_shared<std::vector<size_t>>(iterative_past),
                                 completely_connected_in,
                                 completely_connected_out,
                                 independent_from_in,
                                 independent_from_out,
                                 oct_auxiliary_independent_set
                                );


    // Create search tree bag
    SearchTreeBag test_bag;
    test_bag.add_blueprint(test_blueprint);
    test_bag.add_blueprint(test_blueprint2);

    size_t vertex = 73; // arbitrary value, doesn't matter

    // Check for blueprint that shouldn't be there
    if ( test_bag.ban_vertex_in_blueprints(vertex, 2) ) {
        std::cout << "SearchTreeBag should not contain blueprint with ID = 2 "<< std::endl;
        error = true;
    }

    // ban vertex in blueprint that should be there
    if ( test_bag.ban_vertex_in_blueprints(vertex, 0) == false ) {
        std::cout << "SearchTreeBag should contain blueprint with ID = 0 "<< std::endl;
        error = true;
    }

    // check what's banned
    for (auto blueprint_from_bag : test_bag) {

        // only check the altered blueprint
        if (blueprint_from_bag.get_id() == 0) {
            if (!blueprint_from_bag.is_banned(vertex)) {
                std::cout << "Blueprint with ID = 0 should ban vertex, but doesn't."<< std::endl;
                error = true;
            }
        }
        else {
            if (blueprint_from_bag.is_banned(vertex)) {
                std::cout << "Blueprint with ID = "<< blueprint_from_bag.get_id()
                << " should not ban vertex, but does."<< std::endl;
                error = true;
            }
        }

    }


    return error;

}
