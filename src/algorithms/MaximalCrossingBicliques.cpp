/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include "AllMaximalIndependentSets.h"
#include "MaximalCrossingBicliques.h"
#include <queue>

/**
 * Updates all blueprints in a bag to ensure they point to
 * the correct search tree bag.
 */
void update_iterative_pointers_in_blueprint(SearchTreeBag & current_bag) {

    std::shared_ptr<std::vector<size_t>> & this_iter_wait = current_bag.get_iterative_wait_ptr();
    std::shared_ptr<OrderedVertexSet> & this_iter_in = current_bag.get_iterative_in_ptr();
    std::shared_ptr<std::vector<size_t>> & this_iter_past = current_bag.get_iterative_past_ptr();

    for (BlueprintLite & temp_blueprint : current_bag) {
        temp_blueprint.update_iterative_ptrs(this_iter_wait,
                                             this_iter_in,
                                             this_iter_past);

    }

}


/**
 *  Handle the ban and delay processes for each bag in the input bag_list.
 */
 void handle_bans(const size_t & blueprint_id,
                  const size_t & expanded_vertex,
                  std::list<SearchTreeBag> & bag_list) {

    // Iterate over each bag among children of current_bag
    for ( SearchTreeBag & temp_bag : bag_list ) {

        // No need to update any bag produced by expanding on a node
        // that comes *after* the vertex getting banned
        if (temp_bag.get_expanded_vertex() >= expanded_vertex) break;

        temp_bag.ban_vertex_in_blueprints(expanded_vertex, blueprint_id);

    } // end FOR updating ban lists
 }


/**
 * Given expansion result info, determine whether the blueprint produces a
 * MIB we output, and whether or not to ban.
 */
 void process_expansion_result(BlueprintLite & current_blueprint,
                               ExpansionResult & expand_result,
                               std::vector<BicliqueLite> & biclique_list,
                               std::unordered_map<std::string,bool> & local_duplicates_table,
                               std::list<SearchTreeBag> & bag_list,
                               SearchTreeBag & next_bag) {

    const auto & expand_result_type = expand_result.get_return_type();

    if (expand_result_type == ExpansionResult::RETURN_TYPE::SUCCESS) {

        current_blueprint.update_iterative_ptrs(next_bag.get_iterative_wait_ptr(),
                                                next_bag.get_iterative_in_ptr(),
                                                next_bag.get_iterative_past_ptr());

        size_t next_future_maximal_vertex = current_blueprint.get_last_vertex();

        // If maximal, add to output list
        if (current_blueprint.is_currently_maximal(next_future_maximal_vertex)) {

            biclique_list.push_back( current_blueprint.get_biclique() );

        }
        // Regardless of maximality, update next_future_maximal_vertex
        current_blueprint.set_next_future_maximal(next_future_maximal_vertex);

        local_duplicates_table[current_blueprint.get_partial_biclique_string()] = true;
        next_bag.add_blueprint(current_blueprint);

    }
    else if (expand_result_type == ExpansionResult::RETURN_TYPE::DISCONTINUE) {
        return;
    }
    else {
        // BAN
        handle_bans(current_blueprint.get_id(),
                    expand_result.get_vertex(),
                    bag_list);
    } // end all ban checks

}


/*
 * Enumerate all maximal induced bicliques with one partition contained
 * entirely within a specified independent set, `independent_set`.
 *
 * Proceed by iterating over every vertex of the input independent set,
 * and making a search_tree_list for each such vertex. Populate that search tree
 * with bag-nodes initialized by independent_sets found in the neighborhood of
 * vertex.
 *
 * @param graph                 A graph.
 * @param independent_set       Must be a independent set within graph.
 */
std::vector<BicliqueLite> maximal_crossing_bicliques(const Graph &graph,
                                                 const OrderedVector &independent_set) {

std::shared_ptr<Graph> graph_ptr = std::make_shared<Graph>(graph);

std::vector<BicliqueLite> max_cross_biclique_list;

// For each vertex of independent_set
for (auto vertex : independent_set) {

    if (graph.get_neighbors(vertex).size() == 0) continue;

    std::vector<size_t> ind_set_at_vertex = independent_set.truncate_before_node_vec(vertex);
    OrderedVertexSet iterative_in((std::vector<size_t>) {vertex});
    std::vector<size_t> iterative_wait = ind_set_at_vertex;
    std::vector<size_t> iterative_past = independent_set.truncate_after_node_vec(vertex);

    SearchTreeBag initial_bag(iterative_wait, iterative_in, iterative_past, vertex);

    std::list<SearchTreeBag> search_tree_list;
    size_t blueprint_id = 0;

    // find all maximal independent sets in the graph induced on v's neighborhood
    const std::vector<size_t> vertex_neighborhood = graph.get_neighbors_vector(vertex);
    Graph induced_neighborhood_graph = graph.subgraph(vertex_neighborhood);

    // Need to map subgraph vertex labels to outer labels
    std::vector<std::vector<size_t>> maximal_independent_sets = get_all_mis(induced_neighborhood_graph);
    // Get subgraph-to-graph label converter
    convert_node_labels_vector_inplace(maximal_independent_sets, vertex_neighborhood);

    // For each MIS in vertex's neighborhood, create blueprints
    for (auto & MIS_instance : maximal_independent_sets) {

        // Skip MIS if any prior IS node is completely connected to it
        bool skip_this_MIS = false;
        for (auto u : independent_set) {
            if (u >= vertex) break;
            if (graph.is_completely_connect_to(u, MIS_instance)) {
                skip_this_MIS = true;
                break;
            }
        }

        if (skip_this_MIS) continue;

        std::vector<size_t> CC_in = MIS_instance;
        std::vector<size_t> CC_out(vertex_neighborhood.size());  // compute vertex_neighb - MIS_instance
        auto it = std::set_difference(vertex_neighborhood.begin(),
                                      vertex_neighborhood.end(),
                                      MIS_instance.begin(),
                                      MIS_instance.end(),
                                      CC_out.begin());
        CC_out.resize(it - CC_out.begin());

        std::vector<size_t> IF_in;
        std::vector<size_t> IF_out;
        std::vector<size_t> OCT_aux;

        BlueprintLite this_blueprint(
                                    blueprint_id,
                                    graph_ptr,
                                    initial_bag.get_iterative_wait_ptr(),
                                    initial_bag.get_iterative_in_ptr(),
                                    initial_bag.get_iterative_past_ptr(),
                                    CC_in,
                                    CC_out,
                                    IF_in,
                                    IF_out,
                                    OCT_aux
                                );

        blueprint_id++;
        initial_bag.add_blueprint(this_blueprint);
        size_t next_future_max_vert = this_blueprint.get_last_vertex();
        // Add maximal bicliques to output list
        if (this_blueprint.is_currently_maximal(next_future_max_vert)) {

            max_cross_biclique_list.push_back( this_blueprint.get_biclique() );
        }
        this_blueprint.set_next_future_maximal(next_future_max_vert);

    }

    if (initial_bag.size() > 0) search_tree_list.push_back(initial_bag);

    // For each bag in search_tree_list...
    while (search_tree_list.size() > 0) {

        SearchTreeBag & current_bag = search_tree_list.front();

        // Skip empty bags
        if (current_bag.size() == 0) continue;

        // update the iterative_set pointers for all blueprints in this bag.
        update_iterative_pointers_in_blueprint(current_bag);
        std::vector<size_t> & this_iter_wait = current_bag.get_iterative_wait();
        OrderedVertexSet & this_iter_in = current_bag.get_iterative_in();
        std::vector<size_t> & this_iter_past = current_bag.get_iterative_past();

        // Initialize empty vector to hold all children bags of current_bag
        std::list<SearchTreeBag> child_bag_list;
        std::unordered_map<std::string,bool> local_duplicates_table;

        // iterative_wait is the same for all blueprints in a bag together
        const std::vector<size_t> & nodes_to_expand_on = current_bag.get_iterative_wait();

        // For each node in iterative wait...
        for (const size_t expand_node : nodes_to_expand_on) {

            SearchTreeBag child_bag(this_iter_wait,
                                    this_iter_in,
                                    this_iter_past,
                                    expand_node);

            child_bag.expand_on_vertex(expand_node);

            // For each blueprint in the current bag...
            for (const BlueprintLite & temp_blueprint : current_bag) {

                // If expand_node is beyond the next node needed for maximality, skip
                // If expand_node is banned in blueprint, skip
                if (temp_blueprint.skip_expanding_vertex(expand_node)) continue;

                BlueprintLite current_blueprint = temp_blueprint;

                ExpansionResult expand_result = current_blueprint.expand(expand_node, local_duplicates_table);

                // Expansion performed --
                // Now update BAN info, add to mcb_list if applicable
                process_expansion_result(current_blueprint,
                                         expand_result,
                                         max_cross_biclique_list,
                                         local_duplicates_table,
                                         child_bag_list,
                                         child_bag);

            }
            // If child_bag non empty, add to temp list
            if (child_bag.size() > 0) child_bag_list.push_back(child_bag);

        } // end FOR over nodes to expand current search tree bag on.

        search_tree_list.pop_front();

        // Done creating children of current_bag;
        // add non-empty children to search_tree.
        if (child_bag_list.size() > 0) {
            search_tree_list.splice(search_tree_list.begin(), child_bag_list);
        }

    } // WHILE loop over search_tree_list


} // FOR loop over independent_set

return max_cross_biclique_list;

} // end outer-most function
