# Bicliques project CMake file

cmake_minimum_required(VERSION 3.9)


project(bicliques)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")


# Add the main Bicliques executable. This generates the Bicliques binary, using the
# main file and all other included source files.
add_executable(bicliques
        src/bicliques_algorithms.cpp
        src/graph/Graph.cpp
        src/graph/Graph.h
        src/graph/OctGraph.cpp
        src/graph/OctGraph.h
        src/graph/OrderedVertexSet.cpp
        src/graph/OrderedVertexSet.h
        src/graph/OrderedVector.cpp
        src/graph/OrderedVector.h
        src/graph/BlueprintLite.cpp
        src/graph/BlueprintLite.h
        src/graph/BicliqueLite.cpp
        src/graph/BicliqueLite.h
        src/graph/SearchTreeBag.cpp
        src/graph/SearchTreeBag.h
        src/graph/SearchTreeBagPlus.cpp
        src/graph/SearchTreeBagPlus.h
        src/algorithms/MaximalCrossingBicliques.cpp
        src/algorithms/MaximalCrossingBicliques.h
        src/algorithms/MaximalCrossingBicliquesBipartite.cpp
        src/algorithms/MaximalCrossingBicliquesBipartite.h
        src/algorithms/OCTMIB.cpp
        src/algorithms/OCTMIB.h
        src/algorithms/AllMaximalIndependentSets.cpp
        src/algorithms/AllMaximalIndependentSets.h
        src/algorithms/LexMIB.cpp
        src/algorithms/LexMIB.h
	src/algorithms/iOCTMIB.h
	src/algorithms/iOCTMIB.cpp
        src/algorithms/EnumMIB.cpp
        src/algorithms/EnumMIB.h
        src/algorithms/SimpleCCs.cpp
        src/algorithms/SimpleCCs.h
        src/algorithms/SimpleOCT.cpp
        src/algorithms/SimpleOCT.h
        src/algorithms/GraphOrdering.h
        src/graph/ExpansionResult.cpp
        src/graph/ExpansionResult.h
        src/graph/EditableGraph.cpp
        src/graph/EditableGraph.h
        test/ground_truth_mibs.h
        )


enable_testing()

###############


# Scrape the source list for the bicliques executable and save it as bicliques_source,
# filtering out the main file. We do this so we can reference source files for testing
# without having to duplicate the list.
get_target_property(bicliques_source bicliques SOURCES)
list(FILTER bicliques_source EXCLUDE REGEX bicliques_algorithms.cpp)


# Create a test source list. This adds all files containing tests and generates
# test_driver.cpp, which is eventually used for running tests. Then create a test_driver
# executable, including both test files and main bicliques source files.
create_test_sourcelist(testsrclist
        test_driver.cpp
        test/graph/add_edge.cpp
        test/graph/list_constructor.cpp
        test/graph/edgelist_constructor.cpp
        test/graph/adjlist_constructor.cpp
        test/graph/get_neighbors.cpp
        test/graph/subgraph.cpp
        test/graph/subgraph_permutations.cpp
        test/graph/is_completely_connect_to.cpp
        test/graph/is_completely_independent_from.cpp
        test/graph/biclique_string.cpp
        test/graph/get_neighborhood_intersection.cpp
        test/graph/is_biclique.cpp
        test/graph/can_be_added_to_biclique.cpp
        test/orderedvertexset/list_constructor.cpp
        test/orderedvertexset/map_constructor.cpp
        test/orderedvertexset/set_union.cpp
        test/orderedvertexset/operator_eq.cpp
        test/orderedvertexset/operator_bracket.cpp
        test/orderedvertexset/set_intersection.cpp
        test/orderedvertexset/set_minus.cpp
        test/orderedvertexset/truncate_before_node.cpp
        test/orderedvertexset/vertex_list_operations.cpp
        test/orderedvertexset/range_bounds.cpp
        test/orderedvertexset/has_subset.cpp
        test/orderedvertexset/truncate_after_node.cpp
        test/orderedvertexset/add_vertex_safe.cpp
        test/orderedvertexset/operator_less.cpp
        test/orderedvector/has_vertex.cpp
        test/orderedvector/list_constructor.cpp
        test/orderedvector/operator_bracket.cpp
        test/orderedvector/operator_eq.cpp
        test/orderedvector/operator_less.cpp
        test/orderedvector/vertex_list_operations.cpp
        test/orderedvector/set_union.cpp
        test/orderedvector/vector_intersect_neighborhood.cpp
        test/orderedvector/vector_minus_neighborhood.cpp
        test/octgraph/graph_constructor.cpp
        test/octgraph/get_partition.cpp
        test/octgraph/get_node_partition.cpp
        test/octgraph/get_partition_neighbors.cpp
        test/biclique/operator_eq.cpp
        test/biclique/get_partitions.cpp
        test/biclique/size.cpp
        test/biclique/to_string.cpp
        test/biclique/operator_less.cpp
        test/blueprintlite/constructor.cpp
        test/blueprintlite/next_future_maximal.cpp
        test/blueprintlite/is_banned.cpp
        test/blueprintlite/is_currently_maximal.cpp
        test/blueprintlite/get_bicliques.cpp
        test/blueprintlite/expand_failure_empty_cc_in.cpp
        test/blueprintlite/expand_duplicate_check.cpp
        test/blueprintlite/expand_non_relevant_check.cpp
        test/blueprintlite/expand_future_maximal.cpp
        test/blueprintlite/expand_success.cpp
        test/blueprintlite/expand_add_from_iterative_wait.cpp
        test/searchtreebag/add_blueprint.cpp
        test/searchtreebag/ban_vertex_in_blueprints.cpp
        test/searchtreebag/constructor.cpp
        test/searchtreebagplus/all_functions.cpp
        test/searchtreebagplus/get_biclique.cpp
        test/searchtreebagplus/expand_success.cpp
        test/searchtreebagplus/expand_add_from_iterative_wait.cpp
        test/searchtreebagplus/expand_future_maximal.cpp
        test/searchtreebagplus/expand_failure_empty_cc_in.cpp
        test/expansionresult/constructor.cpp
        test/editablegraph/constructor.cpp
        test/editablegraph/delete_node.cpp
        test/editablegraph/degree_buckets.cpp
        test/allmaximalis/get_all_mis.cpp
        test/allmaximalis/get_all_mis_disconnected.cpp
        test/maximalcrossingbicliques/mcb_bipartite.cpp
        test/maximalcrossingbicliques/mcb_bipartite_medium.cpp
        test/maximalcrossingbicliques/mcb_bipartite_large.cpp
        test/maximalcrossingbicliques/mcb.cpp
        test/maximalcrossingbicliques/mcb_large.cpp
        test/maximalcrossingbicliques/mcb_huge.cpp
        test/lexmib/lex_least_biclique.cpp
        test/lexmib/check_for_mib.cpp
        test/lexmib/lexmib.cpp
        test/lexmib/lexmib_multi_ccs.cpp
        test/lexmib/lexmib_medium.cpp
        test/lexmib/lexmib_large.cpp
        test/lexmib/count_only_multi_ccs.cpp
        test/simpleoct/simpleoct.cpp
        test/simpleoct/simpleoct_bipartite.cpp
        test/simpleoct/simpleoct_bipartite_huge.cpp
        test/simpleoct/simpleoct_robust.cpp
        test/simpleoct/simpleoct_robust_large.cpp
        test/simpleoct/simpleoct_robust_huge.cpp
        test/simpleoct/prescribed_oct_robust_large.cpp
        test/simpleccs/simpleccs_robust.cpp
        test/octmib/octmib_multi_ccs.cpp
        test/octmib/octmib.cpp
        test/octmib/octmib_medium.cpp
        test/octmib/octmib_large.cpp
        test/octmib/octmib_huge.cpp
        test/octmib/oct_set_input.cpp
        )

# Defining test_driver executable, which needs the code base to compile
add_executable(test_driver ${testsrclist} ${bicliques_source})

# Finally, filter test_driver.cpp from the test source list and strip the .cpp file
# extension from the remaining files. Define a test using each test file name, minus
# the extension, with the test_driver executable.
list(FILTER testsrclist EXCLUDE REGEX test_driver.cpp)
foreach (test ${testsrclist})

    # Properly format the test names
    string(REPLACE .cpp "" test_name ${test})
    string(REPLACE test/ "" exec_name ${test_name})
    string(REPLACE / _ exec_name ${exec_name})

    # Add test to ctest list
    add_test(${exec_name} test_driver ${test_name})
endforeach()
