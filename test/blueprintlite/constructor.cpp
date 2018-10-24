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
 * Test that the BlueprintLite constructor functions correctly.
 */
int test_blueprintlite_constructor(int argc, char ** argv) {

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
    std::shared_ptr<Graph> graph_ptr = std::make_shared<Graph>(og);

    // Initialize parameters required to construct blueprintlite
    size_t biclique_blueprint_id = 17;  // an arbitrary test number
    std::vector<size_t> iterative_past;
    OrderedVertexSet iterative_in( (std::vector<size_t>){2} );
    std::vector<size_t> iterative_wait( {4} );


    std::vector<size_t> completely_connected_in( {1} );
    std::vector<size_t> completely_connected_out( {0} );
    std::vector<size_t> independent_from_in( {3} );
    std::vector<size_t> independent_from_out( {5,6} );
    std::vector<size_t> oct_auxiliary_independent_set;

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

    // Test get_biclique_string()
    if ( test_blueprint.get_biclique_string() != "1,2,3,") {
        std::cout << "BlueprintLite constructed wrong: should return '1,2,3,' not " << test_blueprint.get_biclique_string() << std::endl;
        error = true;
    }

    // Test get_id()
    if (test_blueprint.get_id() != biclique_blueprint_id) {
        std::cout << "BlueprintLite constructed wrong: id should be ";
        std::cout << biclique_blueprint_id << " not ";
        std::cout << test_blueprint.get_id() << std::endl;
        error = true;
    }

    return error;

}
