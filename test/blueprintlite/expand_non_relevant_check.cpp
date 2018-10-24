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
 * Test BlueprintLite::expand, termination condition in which the
 * expanded node triggers the non-relevancy checks. This tests both
 * possible outcomes, i.e., both with and without delay lists getting returned.
 */
int test_blueprintlite_expand_non_relevant_check(int argc, char ** argv) {

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

    // Blueprint for expand() to terminate with non-relevant check and a delay list
    size_t biclique_blueprint_id = 0;
    std::vector<size_t> iterative_wait( {7,10} );
    OrderedVertexSet iterative_in( (std::vector<size_t>){3,9} );
    std::vector<size_t> iterative_past( {0} );
    std::vector<size_t> completely_connected_in( {4} );
    std::vector<size_t> completely_connected_out( {2} );
    std::vector<size_t> independent_from_in( {11} );
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

    // empty duplicate list
    std::unordered_map<std::string, bool> duplicates_list = {};

    // Expand on vertex
    size_t expanding_vertex = 10;
    iterative_in.add_vertex_safe(expanding_vertex);
    iterative_wait = (std::vector<size_t>) {7};

    test_blueprint.update_iterative_ptrs(std::make_shared<std::vector<size_t>>(iterative_wait),
                                         std::make_shared<OrderedVertexSet>(iterative_in),
                                         std::make_shared<std::vector<size_t>>(iterative_past));

    ExpansionResult expand_result = test_blueprint.expand(expanding_vertex, duplicates_list);

    // Should return DISCONTINUE
    // check return type
    if (expand_result.get_return_type() != ExpansionResult::RETURN_TYPE::DISCONTINUE ) {
        std::cout << "Expanding on node with non relevant termination should return RETURN_TYPE::DISCONTINUE." << std::endl;
        error = true;
    }

    return error;

}
