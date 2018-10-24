/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include <queue>
#include <list>

#include "MaximalCrossingBicliques.h"
#include "MaximalCrossingBicliquesBipartite.h"
#include "../graph/SearchTreeBagPlus.h"
#include "GraphOrdering.h"

#include "../graph/BicliqueLite.h"

/*
 * Enumerate all maximal induced bicliques with one partition contained
 * entirely within a specified independent set, `indset`.
 *
 * Proceed by iterating over every vertex of the input independent set,
 * and making a search_tree_list for each such vertex. Populate that search tree
 * with bag-nodes initialized by independent_sets found in the neighborhood of
 * vertex.
 *
 * @param graph                 A graph.
 * @param independent_set       Must be a independent set within graph.
 */
void maximal_crossing_bicliques_bipartite(OutputOptions &mcbbresults,
                                          Graph &input_graph,
                                          const std::vector<size_t> &input_left_set,
                                          const std::vector<size_t> &input_right_set) {

// First re-order graph and sets
const auto vertex_ordering = degree_reordering(input_graph);
Graph graph = input_graph.subgraph(vertex_ordering);

mcbbresults.turn_on_relabeling_mode(vertex_ordering);

std::vector<size_t> reverse_ordering(vertex_ordering.size());
for (size_t idx1=0; idx1<vertex_ordering.size(); idx1++) {
    reverse_ordering[vertex_ordering[idx1]] = idx1;
}

// Relabel input sets with new labeling
auto left_set = convert_node_labels_vector(input_left_set, reverse_ordering);
auto right_set = convert_node_labels_vector(input_right_set, reverse_ordering);

// Make sure independent_set is the smaller partition
std::vector<size_t> independent_set = left_set;
if (left_set.size() > right_set.size()) {
    independent_set = right_set;
}

std::shared_ptr<Graph> graph_ptr = std::make_shared<Graph>(graph);


// For each vertex of independent_set
for (const size_t & vertex : independent_set) {

    // find all maximal independent sets in the graph induced on v's neighborhood
    const auto & vertex_neighborhood = graph.get_neighbors_vector(vertex);

    if (vertex_neighborhood.size() == 0) continue;
    OrderedVertexSet MIS_instance = vertex_neighborhood;

    // Skip MIS if any prior IS node is completely connected to it
    bool skip_this_MIS = false;
    for (const size_t & u : independent_set) {
        if (u >= vertex) break;
        if (graph.is_completely_connect_to(u, vertex_neighborhood)) {
            skip_this_MIS = true;
            break;
        }
    }

    if (skip_this_MIS) continue;

    // Initialize first searchtree bag
    std::vector<size_t> iterative_past, ind_set_at_vertex;
    size_t ind_idx = 0;
    for (; ind_idx < independent_set.size(); ind_idx++) {
        size_t this_vertex = independent_set[ind_idx];
        if (this_vertex == vertex) {
            break;
        }
        iterative_past.push_back(this_vertex);
    }
    ind_idx++;
    for (; ind_idx < independent_set.size(); ind_idx++) {
        ind_set_at_vertex.push_back(independent_set[ind_idx]);
    }

    // Add to iterative_in everything from iterative_wait that is
    // completely connected to MIS_instance
    std::vector<size_t> iterative_in;
    iterative_in.push_back(vertex);

    std::vector<size_t> iterative_wait;
    for (const size_t & u: ind_set_at_vertex) {
        if (graph.is_completely_connect_to(u, vertex_neighborhood)) {
            iterative_in.push_back(u);
        }
        else {
            iterative_wait.push_back(u);
        }
    }

    std::vector<size_t> CC_in = vertex_neighborhood;

    SearchTreeBagPlus initial_bag(graph_ptr,
                                  CC_in,
                                  iterative_wait,
                                  iterative_in,
                                  iterative_past,
                                  vertex);

    std::list<SearchTreeBagPlus> search_tree_list;
    size_t blueprint_id = 0;

    mcbbresults.push_back_with_maximality_check(initial_bag.get_biclique());
    search_tree_list.push_back(initial_bag);

    // SEARCH TREE INITIALIZED
    // Now iterate over each bag in search_tree_list...
    while (search_tree_list.size() > 0) {

        SearchTreeBagPlus & current_bag = search_tree_list.front();

        // iterative_wait is the same for all blueprints in a bag together
        const std::vector<size_t> & nodes_to_expand_on = current_bag.get_iterative_wait();

        // Initialize empty vector to hold all children bags of current_bag
        std::list<SearchTreeBagPlus> child_bag_list;

        // For each node we can expand current blueprints on...
        for (const size_t expand_node : nodes_to_expand_on) {

            SearchTreeBagPlus child_bag = current_bag;

            bool expansion_outcome = child_bag.expand(expand_node);

            if (expansion_outcome==true) {
                mcbbresults.push_back_with_maximality_check(child_bag.get_biclique());

                child_bag_list.push_back(child_bag);

            }

        } // end FOR over nodes to expand current search tree bag on.

        search_tree_list.pop_front();

        // Done creating children of current_bag;
        // add non-empty children to search_tree.
        if (child_bag_list.size() > 0) {
            search_tree_list.splice(search_tree_list.begin(), child_bag_list);
        }

    } // WHILE loop over search_tree_list

} // FOR loop over independent_set

} // end outer-most function


std::vector<BicliqueLite> maximal_crossing_bicliques_bipartite(Graph &graph,
                                                const std::vector<size_t> &left,
                                                const std::vector<size_t> &right){

    OutputOptions tempresults;
    maximal_crossing_bicliques_bipartite(tempresults, graph, left, right);

    return tempresults.mibs_computed;
}
