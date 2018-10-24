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
 * Test that BlueprintLite::get_bicliques() checks functions for
 * retrieving biclique partition vectors and printing out biclique strings.
 */
int test_blueprintlite_get_bicliques(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_ban.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Set up octgraph
    OrderedVertexSet oct_partition((std::vector<size_t>){0,1,3,7,9,10});
    OrderedVertexSet left_partition((std::vector<size_t>){8,11,12,13});
    OrderedVertexSet right_partition((std::vector<size_t>){2,4,5,6});

    // Make OctGraph with those partitions
    OctGraph og(g,oct_partition,left_partition,right_partition);
    std::shared_ptr<Graph> graph_ptr = std::make_shared<Graph>(og);

    size_t biclique_blueprint_id = 0;
    std::vector<size_t> iterative_wait( {9,10} );
    OrderedVertexSet iterative_in( (std::vector<size_t>){3,7} );
    std::vector<size_t> iterative_past( {0} );
    std::vector<size_t> completely_connected_in( {4} );
    std::vector<size_t> completely_connected_out( {2} );
    std::vector<size_t> independent_from_in( {11,12} );
    std::vector<size_t> independent_from_out( {13} );
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

    // Test biclique_string
    if ( test_blueprint.get_biclique_string() != "3,4,7,11,12,") {
        std::cout << "BlueprintLite string wrong: should return '3,4,7,11,12,' not " << test_blueprint.get_biclique_string() << std::endl;
        error = true;
    }

    // Test get_biclique
    BicliqueLite true_biclique((std::vector<size_t>) {3,7,11,12} ,
                               (std::vector<size_t>) {4} );

    if ( test_blueprint.get_biclique() != true_biclique ) {
        std::cout << "BlueprintLite biclique constructed wrong" << std::endl;
        error = true;
    }

    // Test partial_biclique_string
    if ( test_blueprint.get_partial_biclique_string() != "4,11,12,") {
        std::cout << "BlueprintLite string wrong: should return '4,11,12,' not " << test_blueprint.get_partial_biclique_string() << std::endl;
        error = true;
    }

    // Test get_biclique
    BicliqueLite true_partial((std::vector<size_t>) {11,12} ,
                               (std::vector<size_t>) {4} );

    if ( test_blueprint.get_partial_biclique() != true_partial ) {

        std::cout << "BlueprintLite partial biclique constructed wrong" << std::endl;
        error = true;

    }

return error;

}
