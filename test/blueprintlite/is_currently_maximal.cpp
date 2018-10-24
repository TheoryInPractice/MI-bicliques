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
 * Test BlueprintLite::is_currently_maximal, make sure it reports
 * True for bicliques that are maximal, and false for bicliques that are
 * not. Note: the check is performed by checking whether vertices from
 * oct_auxiliary_independent_set or iterative_wait can be added to
 * the left partition (i.e. are independent from left, connected to
 * the right partition), so this relies on correctness of the functions
 * that construct the sets in each blueprint.
 */
int test_blueprintlite_is_currently_maximal(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_delay_conditions.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Set up octgraph
    OrderedVertexSet oct_partition((std::vector<size_t>){0,1,3,7,9,10});
    OrderedVertexSet left_partition((std::vector<size_t>){8,11,12,13});
    OrderedVertexSet right_partition((std::vector<size_t>){2,4,5,6});

    // Make OctGraph with those partitions
    OctGraph og(g,oct_partition,left_partition,right_partition);
    std::shared_ptr<Graph> graph_ptr = std::make_shared<Graph>(og);

    // Make blueprintlite
    size_t biclique_blueprint_id = 0;
    std::vector<size_t> iterative_wait( {9,10} );
    std::vector<size_t> iterative_past( {0} );
    OrderedVertexSet iterative_in( (std::vector<size_t>){3,7} );
    std::vector<size_t> completely_connected_in( {4,5} );
    std::vector<size_t> completely_connected_out( {} );
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

    // Should not be maximal now
    if (test_blueprint.is_currently_maximal()) {
        std::cout << "Non-maximal blueprintlite reported as maximal." << std::endl;
        error = true;
    }

    // After expanding on vertex 9, should make biclique maximal
    size_t expanding_vertex = 9;
    iterative_in.add_vertex_safe(expanding_vertex);
    iterative_wait = (std::vector<size_t>) {10};

    test_blueprint.update_iterative_ptrs(std::make_shared<std::vector<size_t>>(iterative_wait),
                                         std::make_shared<OrderedVertexSet>(iterative_in),
                                         std::make_shared<std::vector<size_t>>(iterative_past));


    std::unordered_map<std::string, bool> duplicates_list = {};
    ExpansionResult expand_result = test_blueprint.expand(expanding_vertex,duplicates_list);

    // Should be maximal now
    if (!test_blueprint.is_currently_maximal()) {
        std::cout << "Maximal blueprintlite not reported as maximal:" << std::endl;
        std::cout << test_blueprint.get_biclique_string() << std::endl;
        error = true;
    }

    return error;

}
