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
#include"../../src/graph/BlueprintLite.h"

/**
 * Test that BlueprintLite::is_banned() properly checks the ban
 * status of a vertex in a blueprintlite.
 */
int test_blueprintlite_is_banned(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_ban.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Set up octgraph
    OrderedVertexSet oct_partition((std::vector<size_t>){0,1,2,7,8,9});
    OrderedVertexSet left_partition((std::vector<size_t>){10,11,12});
    OrderedVertexSet right_partition((std::vector<size_t>){3,4,5,6,13});

    // Make OctGraph with those partitions
    OctGraph og(g,oct_partition,left_partition,right_partition);
    std::shared_ptr<Graph> graph_ptr = std::make_shared<Graph>(og);

    // Initialize paremeters required to construct blueprintlite
    size_t biclique_blueprint_id = 0;
    std::vector<size_t> iterative_wait( {9} );
    OrderedVertexSet iterative_in( (std::vector<size_t>){2,8} );
    std::vector<size_t> iterative_past( {0,7} );
    std::vector<size_t> completely_connected_in( {3,4} );
    std::vector<size_t> completely_connected_out( {5,6} );
    std::vector<size_t> independent_from_in( {10,11} );
    std::vector<size_t> independent_from_out( {12,13} );
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

    test_blueprint.ban_vertex(9);

    // Check if vertex 9 is_banned
    if(!test_blueprint.is_banned(9)){
        std::cout << "Vertex 9 should be banned but is not" << std::endl;
        error = true;
    }

    // Verify that all non-banned vertices are such
    for(size_t idx = 0; idx < og.get_num_vertices(); idx++){
        if(idx == 9) continue;
        if(test_blueprint.is_banned(idx)){
            std::cout << "Vertex " << idx <<  " should not be banned but is" << std::endl;
            error = true;
        }
    }

    // Test skip_expanding_vertex on banned vertex
    if(!test_blueprint.skip_expanding_vertex(9)){
        std::cout << "Expanding on vertex 9 should skip, but didn't." << std::endl;
        error = true;
    }
    // Test skip_expanding_vertex on all vertices
    size_t dummy_next_future = 10;
    test_blueprint.set_next_future_maximal(dummy_next_future);
    for (size_t idx = 0; idx < g.get_num_vertices(); idx++){
        if (idx == 9) continue;  // skip over banned vertex
        // Test vertices that should not be skipped
        if (idx <= dummy_next_future) {
            if (test_blueprint.skip_expanding_vertex(idx)){
                std::cout << "Expanding on vertex " << idx;
                std::cout << " should not skip, but did." << std::endl;
                error = true;
            }
        }
        // Test vertices that should be skipped
        else {
            if (!test_blueprint.skip_expanding_vertex(idx)){
                std::cout << "Expanding on vertex " << idx;
                std::cout << " should skip, but didn't." << std::endl;
                error = true;
            }
        }
    }

    return error;

}
