/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include <ctime> // for timing
#include <csignal> // for triggering early termination
#include <list>

#include "OCTMIB.h"
#include "MaximalCrossingBicliques.h"
#include "MaximalCrossingBicliquesBipartite.h"
#include "SimpleCCs.h"

#include "../graph/OrderedVector.h"

/**
 * Given expansion result info, determine whether the blueprint produces a
 * MIB we output, and whether or not to ban.
 */
 void process_expansion_result(OutputOptions & octmib_results,
                               BlueprintLite & current_blueprint,
                               ExpansionResult & expand_result,
                               std::unordered_map<std::string,bool> & global_duplicates_table,
                               std::unordered_map<std::string,bool> & local_duplicates_table,
                               std::list<SearchTreeBag> & bag_list,
                               SearchTreeBag & next_bag) {

    auto expand_result_type = expand_result.get_return_type();

    if (expand_result_type == ExpansionResult::RETURN_TYPE::SUCCESS) {

        current_blueprint.update_iterative_ptrs(next_bag.get_iterative_wait_ptr(),
                                                next_bag.get_iterative_in_ptr(),
                                                next_bag.get_iterative_past_ptr());

        size_t next_future_maximal_vertex = current_blueprint.get_last_vertex();

        // If maximal, add to output list
        if (current_blueprint.is_currently_maximal(next_future_maximal_vertex)) {
            std::string this_blueprint_string = current_blueprint.get_biclique_string();
            if (global_duplicates_table.find(this_blueprint_string) == global_duplicates_table.end()) {
                octmib_results.push_back(current_blueprint.get_biclique());
                global_duplicates_table[this_blueprint_string] = true;
            }
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


 /**
  * Implements our Maximal Induced Biclique algorithm exploiting an OCT set.
  *
  * - Compute OCT decomposition using simple BFS coloring algorithm.
  *
  * - on the biparite portion, use our MCB algorithm to find all bicliques there.
  * - check those bicliques against the OCT set to check for maximality.
  *
  * - Use our implementation of Eppstein's algorithm to compute all MIS in OCT
  * - iterate over MIS in OCT, extract relevant portions from L and R partitions
  * - use our MCB to extract MIBs on relevant subgraph determined this way
  *
  * - heavily uses BlueprintLite::expand
  */
void octmib_with_oct(OutputOptions & octmib_results, OctGraph &og) {

    std::unordered_map<std::string, bool> global_duplicates_table;

    auto graph_ptr = std::make_shared<Graph>(og);

    const OrderedVertexSet & left_partition = og.get_left();
    const OrderedVertexSet & oct_set = og.get_oct();
    const OrderedVertexSet & right_partition = og.get_right();

    const OrderedVector left_partition_OV(left_partition.get_vertices());
    const OrderedVector oct_set_OV(oct_set.get_vertices());
    const OrderedVector right_partition_OV(right_partition.get_vertices());

    OrderedVector non_oct_vertices(left_partition_OV.set_union(right_partition_OV));

    std::vector<size_t> external_labeling;
    bool relabel_flag = false;
    if (octmib_results.relabeling_mode) {
        external_labeling = octmib_results.relabeling_vector;
        relabel_flag = true;
    }

    /**
     * PHASE 1.1: run MCB (or Langston) on the bipartite graph (g - OCT)
     */
     clock_t begin_mcb_timing = std::clock();

    // Case 1: OCT-set empty --> just run MCBB
    if (oct_set.size() == 0) {
        maximal_crossing_bicliques_bipartite(octmib_results,
                                             og,
                                             left_partition.get_vertices(),
                                             right_partition.get_vertices() );
    }
    // Case 2: non-empty OCT-set
    else {

         // Extract bipartite subgraph
        Graph g_minus_oct = og.subgraph(non_oct_vertices);
        std::vector<size_t> bipartite_left, bipartite_right;
        for (size_t idx=0; idx< non_oct_vertices.size(); idx++){
            if (left_partition.has_vertex(non_oct_vertices[idx])) {
                bipartite_left.push_back(idx);
            }
            else bipartite_right.push_back(idx);
        }

        // Compute mcbs on bipartite graph
        octmib_results.turn_on_max_check_mode(global_duplicates_table, oct_set);
        octmib_results.turn_on_relabeling_mode(non_oct_vertices);

        maximal_crossing_bicliques_bipartite(octmib_results,
                                             g_minus_oct,
                                             bipartite_left,
                                             bipartite_right);

    }

    // Reseting the labelling system is required in case MCBB alters labeling
    octmib_results.turn_off_relabeling_mode();
    octmib_results.turn_off_max_check_mode();
    if (relabel_flag) {
        octmib_results.turn_on_relabeling_mode(external_labeling);
    }

    clock_t end_mcb_timing = std::clock();
    octmib_results.time_bipartite_mcb += double(end_mcb_timing - begin_mcb_timing) / CLOCKS_PER_SEC;
    // if no oct set, the graph is bipartite -- return

    if (oct_set.size() == 0) return;


    /**
     * PHASE 2: find MIBS on the rest of the input graph
     */
    // Compute MIS on graph induced by OCT set
    clock_t begin_oct_mis_timing = std::clock();

    auto oct_subgraph = og.subgraph(oct_set);
    auto all_mis_in_oct = get_all_mis(oct_subgraph);
    // Convert each MIS to use labelling of graph instead of subgraph(oct_set).
    convert_node_labels_vector_inplace(all_mis_in_oct, oct_set);

    // Update the OCTMIB results object
    octmib_results.num_oct_mis += all_mis_in_oct.size();
    octmib_results.num_oct_edges += oct_subgraph.get_num_edges();

    clock_t end_oct_mis_timing = std::clock();
    octmib_results.time_oct_MIS += double(end_oct_mis_timing - begin_oct_mis_timing) / CLOCKS_PER_SEC;


    // Process each MIS in OCT separately. For each MIS in OCT,
    // for each node in the MIS, initialize blueprints

    // First, compute sum of sizes of MISes, for statistics tracking purposes.
    for (auto & mis_in_oct : all_mis_in_oct) {
            octmib_results.num_oct_iter_mis += mis_in_oct.size();
    }

    // For each maximal independent set mis of OCT
    for (auto & temp_mis_in_oct : all_mis_in_oct) {

        OrderedVector mis_in_oct(temp_mis_in_oct);

        // For each vertex v of MIS found in OCT
        for (auto v : mis_in_oct) {

            // Initialization of search-tree
            std::list<SearchTreeBag> search_tree_list;
            size_t blueprint_id = 0;

            // Compute sets relevant to v, neighb, and oct, l ,r
            OrderedVector v_neighb_oct_OV(og.get_oct_neighbors(v).get_vertices());
            OrderedVector v_neighb_left_OV(og.get_left_neighbors(v).get_vertices());
            OrderedVector v_neighb_right_OV(og.get_right_neighbors(v).get_vertices());

            OrderedVector dummy_temp_OV = oct_set_OV.set_minus(v_neighb_oct_OV);
            std::vector<size_t> oct_non_v_neighb_non_mis = dummy_temp_OV.set_minus(temp_mis_in_oct);
            dummy_temp_OV.clear();

            // Compute all MIS in subgraph induced by v's neighborhood
            const auto & v_neighb = og.get_neighbors(v);
            OrderedVector v_neighb_OV(v_neighb.get_vertices());
            Graph v_neighb_subgraph = og.subgraph(v_neighb);

            clock_t begin_mis_timing = std::clock();

            auto all_mis_in_v_neighb = get_all_mis(v_neighb_subgraph);

            convert_node_labels_vector_inplace(all_mis_in_v_neighb, v_neighb);

            clock_t end_mis_timing = std::clock();
            octmib_results.time_iter_mis += double(end_mis_timing - begin_mis_timing) / CLOCKS_PER_SEC;

            // Compute relevant MIS information to initialize blueprints
            OrderedVertexSet v_exclusion_set = v_neighb.set_union(oct_set); // useful as OVS

            OrderedVector non_oct_non_v_neighb_vertices = non_oct_vertices;
            non_oct_non_v_neighb_vertices.vector_minus_neighborhood(og, v);


            std::vector<size_t> mis_in_oct_truncate_after_v = mis_in_oct.truncate_after_node_vec(v);

            OrderedVertexSet iterative_in((std::vector<size_t>) {v});
            std::vector<size_t> iterative_past = mis_in_oct_truncate_after_v;
            std::vector<size_t> iterative_wait = mis_in_oct.truncate_before_node_vec(v);
            SearchTreeBag initial_bag(iterative_wait,
                                      iterative_in,
                                      iterative_past,
                                      v);

            // for each MIS in the neighborhood of v
            // for (auto & mis_in_v_neighb: all_mis_in_v_neighb) {
            while (all_mis_in_v_neighb.size() > 0) {
                auto & mis_in_v_neighb = all_mis_in_v_neighb.back();

                clock_t begin_blueprint_init_timing = std::clock();

                bool skip_this_check = false;
                for (auto temp_v: mis_in_oct_truncate_after_v) {
                    if (og.is_completely_connect_to(temp_v, mis_in_v_neighb)){
                        // continue;
                        skip_this_check = true;
                        break;
                    }
                }
                if (skip_this_check) {
                    all_mis_in_v_neighb.pop_back();
                    continue;
                }

                // We do not want to store blueprints more than once so we check to see
                // if any nodes from the left or right partitions could be added to the biclique
                // which includes v and the MIS in v's neighborhood

                // Get all nodes in left or right, that are not neighbors of v,
                // but *are* neighbors with all nodes in mis_in_v_neighb

                OrderedVector temp_set(og.get_neighborhood_intersection(mis_in_v_neighb,
                                                             v_exclusion_set));

                auto temp_left = temp_set.set_intersection(left_partition.get_vertices());
                auto temp_right = temp_set.set_intersection(right_partition.get_vertices());

                // Skip these cases because we'll find these MIBs via MCB below
                if (temp_left.size() > 0 && temp_right.size() == 0) {
                    // continue;
                    skip_this_check = true;
                }
                else if (temp_left.size() == 0 && temp_right.size() > 0) {
                    // continue;
                    skip_this_check = true;
                }

                // In this case we will not find this blueprint elsewhere so we initialize a
                // blueprint here.
                else if (temp_left.size() == 0 && temp_right.size() == 0) {

                    BlueprintLite this_blueprint(
                                                blueprint_id,
                                                graph_ptr,
                                                initial_bag.get_iterative_wait_ptr(),
                                                initial_bag.get_iterative_in_ptr(),
                                                initial_bag.get_iterative_past_ptr(),
                                                mis_in_v_neighb,
                                                v_neighb_OV.set_minus(mis_in_v_neighb),
                                                (std::vector<size_t>) {},
                                                non_oct_non_v_neighb_vertices.get_vertices(),
                                                oct_non_v_neighb_non_mis
                                            );

                    blueprint_id++;
                    initial_bag.add_blueprint(this_blueprint);

                }
                else {
                // mis_in_v_neighb has neighbors in both Left and Right which
                // are not neighbors with v here we check if we will find a
                // biclique which contains this MIS in another part of the algorithm

                    bool temp_left_node_ind_from_temp_right = false;
                    for (auto this_node: temp_left) {
                        // if no node from temp_left is completely indepenent
                        // from temp_right, then we will find a biclique which
                        // contains the MIS later on
                        if (og.is_completely_independent_from(this_node, temp_right)) {
                            temp_left_node_ind_from_temp_right = true;
                            break;
                        }
                    }
                    if (temp_left_node_ind_from_temp_right) {
                        bool temp_right_node_ind_from_temp_left = false;
                        // if no node from temp_right is completely independent
                        // from temp_left, then we will find a biclique which
                        // contains the MIS later on
                        for (auto this_node: temp_right) {
                            if (og.is_completely_independent_from(this_node, temp_left)) {
                                temp_right_node_ind_from_temp_left = true;
                                break;
                            }
                        }

                        // otherwise we find a biclique which contains this MIS now,
                        // as it will not be found elsewhere
                        if (temp_right_node_ind_from_temp_left) {

                            // Get MIS from temp_set: find all the nodes from
                            // temp_right which are completely independent from temp_left
                            OrderedVector temp_right_ind_from_left;
                            for (auto this_node: temp_right) {
                                if (og.is_completely_independent_from(this_node, temp_left)){
                                    temp_right_ind_from_left.push_back(this_node);
                                }
                            }

                            std::vector<size_t> mis_in_temp_set = temp_right_ind_from_left.set_union(temp_left);

                            // create blueprint for biclique which contains the MIS
                            BlueprintLite this_blueprint(
                                                        blueprint_id,
                                                        graph_ptr,
                                                        initial_bag.get_iterative_wait_ptr(),
                                                        initial_bag.get_iterative_in_ptr(),
                                                        initial_bag.get_iterative_past_ptr(),
                                                        mis_in_v_neighb,
                                                        v_neighb_OV.set_minus(mis_in_v_neighb),
                                                        mis_in_temp_set,
                                                        non_oct_non_v_neighb_vertices.set_minus(mis_in_temp_set),
                                                        oct_non_v_neighb_non_mis
                                                        );

                            blueprint_id++;
                            initial_bag.add_blueprint(this_blueprint);

                        }
                    }
                }

                clock_t end_blueprint_init_timing = std::clock();
                octmib_results.time_blueprint_init += double(end_blueprint_init_timing - begin_blueprint_init_timing) / CLOCKS_PER_SEC;

                all_mis_in_v_neighb.pop_back();
            } // end FOR over inner mis

            /**
            * Now call MCB on subgraphs. We will find bicliques where v is
            * independent from all left nodes in the biclique and completely
            * connected to all right and OCT nodes; and then find bicliques
            * where v is independent from all right nodes in the biclique and
            * completely connected to all left and OCT nodes.
            */
            clock_t begin_mcb_timing = std::clock();

            // we begin by making the subgraphs
            OrderedVector right_non_v_neighb = right_partition_OV.set_minus(v_neighb_OV);  // needs to be OV for has_vertex
            OrderedVector left_non_v_neighb = left_partition_OV.set_minus(v_neighb_OV);  // needs to be OV for has_vertex

            // Get subgraph on (v's neighbs in oct and right + non-neighbs in left)
            std::vector<size_t> v_oct_right_neighb = v_neighb_oct_OV.set_union(v_neighb_right_OV);
            OrderedVector v_oct_right_neighb_non_left_neighb = left_non_v_neighb.set_union(v_oct_right_neighb);

            Graph subgraph_v_oct_right_neighb_non_left_neighb = og.subgraph(v_oct_right_neighb_non_left_neighb);

            // Must convert global labels to subgraph labels.
            OrderedVector subgraph_labels_for_left_non_v_neighb;
            for (size_t temp_i=0; temp_i < v_oct_right_neighb_non_left_neighb.size(); temp_i++) {
                size_t temp_global_idx = v_oct_right_neighb_non_left_neighb[temp_i];
                if (left_non_v_neighb.has_vertex(temp_global_idx)) {
                    subgraph_labels_for_left_non_v_neighb.push_back(temp_i);
                }
            }

            auto mcbs_list = maximal_crossing_bicliques(subgraph_v_oct_right_neighb_non_left_neighb,
                                       subgraph_labels_for_left_non_v_neighb);

            convert_node_labels_biclique(mcbs_list,
                                         v_oct_right_neighb_non_left_neighb);

            clock_t end_mcb_timing = std::clock();
            octmib_results.time_mcbs += double(end_mcb_timing - begin_mcb_timing) / CLOCKS_PER_SEC;

            clock_t begin_mcb_checking = std::clock();
            // for each mcb which is found, we check to see if a right non-neighbor of v can be added to the side of the mcb containing
            // left nodes. If it can we discard the biclique as we will have found a corresponding blueprint elsewhere. Otherwise
            // we create a blueprint for this biclique.
            while (mcbs_list.size() > 0) {
                const BicliqueLite & this_mcb = mcbs_list.back();

                bool node_can_be_added_left = false;
                const std::vector<size_t> & this_mcb_left = this_mcb.get_left();
                const std::vector<size_t> & this_mcb_right = this_mcb.get_right();

                for (auto j: right_non_v_neighb) {

                    if ( std::binary_search(this_mcb_left.begin(),
                                            this_mcb_left.end(), j) ) continue;
                    if ( std::binary_search(this_mcb_right.begin(),
                                            this_mcb_right.end(), j) ) continue;

                    if (og.is_completely_independent_from(j, this_mcb_left)
                        && og.is_completely_connect_to(j, this_mcb_right) ) {

                        node_can_be_added_left = true;
                        break;
                    }
                }
                if (!node_can_be_added_left) {
                    BlueprintLite this_blueprint(blueprint_id,
                                                 graph_ptr,
                                                 initial_bag.get_iterative_wait_ptr(),
                                                 initial_bag.get_iterative_in_ptr(),
                                                 initial_bag.get_iterative_past_ptr(),
                                                 this_mcb_right,
                                                 v_neighb.set_minus_vec(this_mcb_right),
                                                 this_mcb_left,
                                                 non_oct_non_v_neighb_vertices.set_minus(this_mcb_left),
                                                 oct_non_v_neighb_non_mis
                                                );

                    blueprint_id++;
                    initial_bag.add_blueprint(this_blueprint);
                }

                mcbs_list.pop_back();
            }

            clock_t end_mcb_checking = std::clock();
            octmib_results.time_mcb_checking += double(end_mcb_checking - begin_mcb_checking) / CLOCKS_PER_SEC;


            begin_mcb_timing = std::clock();
            // We then repeat the process on the reversed graph.
            // Get subgraph on (v's neighbs in oct and left + non-neighbs in right)
            std::vector<size_t> v_oct_left_neighb = v_neighb_oct_OV.set_union(v_neighb_left_OV);
            std::vector<size_t> v_oct_left_neighb_non_right_neighb = right_non_v_neighb.set_union(v_oct_left_neighb);

            Graph subgraph_v_oct_left_neighb_non_right_neighb = og.subgraph(v_oct_left_neighb_non_right_neighb);

            // Must convert global labels to subgraph labels.
            std::vector<size_t> subgraph_labels_for_right_non_v_neighb;
            for (size_t temp_i=0; temp_i < v_oct_left_neighb_non_right_neighb.size(); temp_i++) {
                size_t temp_global_idx = v_oct_left_neighb_non_right_neighb[temp_i];
                if (right_non_v_neighb.has_vertex(temp_global_idx)) {
                    subgraph_labels_for_right_non_v_neighb.push_back(temp_i);
                }
            }

            mcbs_list = maximal_crossing_bicliques(subgraph_v_oct_left_neighb_non_right_neighb,
                                       subgraph_labels_for_right_non_v_neighb);

            convert_node_labels_biclique(mcbs_list,
                                         v_oct_left_neighb_non_right_neighb);

            end_mcb_timing = std::clock();
            octmib_results.time_mcbs += double(end_mcb_timing - begin_mcb_timing) / CLOCKS_PER_SEC;


            begin_mcb_checking = std::clock();
            // for each mcb which is found, we check to see if a left non-neighbor of v can be added to the side of the mcb containing
            // right nodes. If it can we discard the biclique as we will have found a corresponding blueprint elsewhere. Otherwise
            // we create a blueprint for this biclique.
            while (mcbs_list.size() > 0) {
                BicliqueLite & this_mcb = mcbs_list.back();

                bool node_can_be_added_left = false;
                const std::vector<size_t> & this_mcb_left = this_mcb.get_left();
                const std::vector<size_t> & this_mcb_right = this_mcb.get_right();

                for (auto j: left_non_v_neighb) {

                    if ( std::binary_search(this_mcb_left.begin(),
                                            this_mcb_left.end(), j) ) continue;
                    if ( std::binary_search(this_mcb_right.begin(),
                                            this_mcb_right.end(), j) ) continue;

                    if (og.is_completely_independent_from(j, this_mcb_left)
                        && og.is_completely_connect_to(j, this_mcb_right) ) {

                        node_can_be_added_left = true;
                        break;
                    }
                }
                if (!node_can_be_added_left) {
                    BlueprintLite this_blueprint(blueprint_id,
                                                 graph_ptr,
                                                 initial_bag.get_iterative_wait_ptr(),
                                                 initial_bag.get_iterative_in_ptr(),
                                                 initial_bag.get_iterative_past_ptr(),
                                                 this_mcb_right,
                                                 v_neighb.set_minus_vec(this_mcb_right),
                                                 this_mcb_left,
                                                 non_oct_non_v_neighb_vertices.set_minus(this_mcb_left),
                                                 oct_non_v_neighb_non_mis
                                                );

                    blueprint_id++;
                    initial_bag.add_blueprint(this_blueprint);
                }

                mcbs_list.pop_back();
            }

            // Now that we have populated initial bag with blueprints, we will check them for future maximality
            // and then maximality if applicable
            SearchTreeBag future_maximal_bag(iterative_wait, iterative_in, iterative_past, v);
            while (initial_bag.size() > 0) {
                BlueprintLite & this_blueprint = initial_bag.back();

                const std::vector<size_t> & iter_past = initial_bag.get_iterative_past();

                // If anything from iter_past can be added to blueprint,
                // skip that blueprint -- not future maximal
                // otherwise add it to future maximal bag
                if (this_blueprint.can_be_added_left(iter_past)) {
                    initial_bag.pop_back();
                    continue;
                }

                this_blueprint.update_iterative_ptrs(future_maximal_bag.get_iterative_wait_ptr(),
                                                     future_maximal_bag.get_iterative_in_ptr(),
                                                     future_maximal_bag.get_iterative_past_ptr());

                future_maximal_bag.add_blueprint(this_blueprint);


                // now we check for maximality, if it is maximal we update appropriately
                size_t next_future_maximal_vertex = this_blueprint.get_last_vertex();
                if (this_blueprint.is_currently_maximal(next_future_maximal_vertex)) {
                    auto this_blueprint_string = this_blueprint.get_biclique_string();
                    if (global_duplicates_table.find(this_blueprint_string) == global_duplicates_table.end()) {
                        octmib_results.push_back(this_blueprint.get_biclique());
                        global_duplicates_table[this_blueprint_string] = true;
                    }
                }
                // Regardless of maximality, update next_future_maximal_vertex
                this_blueprint.set_next_future_maximal(next_future_maximal_vertex);

                initial_bag.pop_back();
            }

            end_mcb_checking = std::clock();
            octmib_results.time_mcb_checking += double(end_mcb_checking - begin_mcb_checking) / CLOCKS_PER_SEC;

            // as long as the bag is not empty we add it to our queue
            if (future_maximal_bag.size() > 0) {
                search_tree_list.push_back(future_maximal_bag);
            }
            future_maximal_bag.clear();

            clock_t begin_search_tree_expand = std::clock();
            // NOW BEGIN EXPANSION
            while ( search_tree_list.size() > 0 ) {

                SearchTreeBag & current_bag = search_tree_list.front();

                // Skip empty bags
                if (current_bag.size() == 0) {
                    search_tree_list.pop_front();
                    continue;
                }

                // update the iterative_set pointsers for all blueprints in this bag.
                update_iterative_pointers_in_blueprint(current_bag);
                std::vector<size_t> & this_iter_wait = current_bag.get_iterative_wait();
                OrderedVertexSet & this_iter_in = current_bag.get_iterative_in();
                std::vector<size_t> & this_iter_past = current_bag.get_iterative_past();

                // Initialize empty vector to hold all children bags of current_bag
                std::list<SearchTreeBag> child_bag_list;
                std::unordered_map<std::string,bool> local_duplicates_table;

                // iterative_wait, _in are the same for all blueprints in a bag together
                const std::vector<size_t> & nodes_to_expand_on = current_bag.get_iterative_wait();
                size_t last_iter_in_node = this_iter_in.back();

                for (size_t expand_node: nodes_to_expand_on) {

                    SearchTreeBag child_bag(this_iter_wait,
                                            this_iter_in,
                                            this_iter_past,
                                            expand_node);

                    child_bag.expand_on_vertex(expand_node);

                    for (const BlueprintLite & this_blueprint: current_bag) {

                        // If expand_node is beyond the next node needed for maximality, skip
                        // If expand_node is banned in blueprint, skip
                        if (this_blueprint.skip_expanding_vertex(expand_node)) continue;

                        // copy blueprint because it needs to be reused
                        BlueprintLite temp_blueprint = this_blueprint;

                        ExpansionResult expand_result = temp_blueprint.expand(expand_node, local_duplicates_table);

                        // Expansion performed --
                        // Now update BAN info, add to mcb_list if applicable
                        process_expansion_result(octmib_results,
                                                 temp_blueprint,
                                                 expand_result,
                                                 global_duplicates_table,
                                                 local_duplicates_table,
                                                 child_bag_list,
                                                 child_bag);

                    } // end FOR over blueprints in current bag

                    // If bag non-empty, add to child_bag_list
                    if (child_bag.size() > 0) child_bag_list.push_back(child_bag);

                } // end FOR over nodes to expand current search tree bag on.

                search_tree_list.pop_front();

                // Done creating children of current_bag;
                // add non-empty children to search_tree.
                if (child_bag_list.size() > 0) {
                    search_tree_list.splice(search_tree_list.begin(), child_bag_list);
                }

            } // end WHILE over search_tree_list

            clock_t end_search_tree_expand = std::clock();
            octmib_results.time_search_tree_expand += double(end_search_tree_expand - begin_search_tree_expand) / CLOCKS_PER_SEC;

        octmib_results.num_oct_iter_mis_completed++;

        // If MIB limit is in place, check for early termination
        if (octmib_results.mib_limit_flag) {
            if (octmib_results.total_num_mibs > octmib_results.mib_limit_value) {
                raise(SIGINT);
            }
        }

        } // FOR loop over each vertex in MIS

        octmib_results.num_oct_mis_completed++;
    } // FOR loop over each MIS in OCT

}


/**
 * Wrapper for our OCTMIB algorithm together that first separates out
 * connected components, runs our algorithm on each CC, and aggregates
 * the results.
 */
void octmib(OutputOptions & octmib_results,
             const Graph & g,
             OrderedVertexSet input_oct_set,
             OrderedVertexSet input_left_set,
             OrderedVertexSet input_right_set) {


    octmib_results.set_base_graph(g);
    octmib_results.n = g.get_num_vertices();
    octmib_results.m = g.get_num_edges();

    // Determine connected components
    clock_t begint = std::clock();
    std::vector<std::vector<size_t>> vector_of_ccs = simpleccs(g);
    clock_t endt = std::clock();
    octmib_results.time_ccs += double(endt - begint) / CLOCKS_PER_SEC;
    octmib_results.num_connected_components = vector_of_ccs.size();

    std::cout << "# Graph has " << vector_of_ccs.size();
    std::cout << " connected components. CC ran in " << octmib_results.time_ccs;
    std::cout << std::endl;

    // Get correct number edges in prescribed OCT decomp, if provided
    if (input_oct_set.size() > 0) {
        Graph temp_graph = g.subgraph(input_oct_set);
        octmib_results.num_oct_edges_given = temp_graph.get_num_edges();
    }


    if (vector_of_ccs.size() > 1) {
        OrderedVertexSet oct_set_cc, left_set_cc, right_set_cc;

        // run octmib on each CC
        for (size_t which_cc = 0; which_cc < vector_of_ccs.size(); which_cc++) {

            // Time this CC
            begint = std::clock();

            std::vector<size_t> vertex_subset = vector_of_ccs[which_cc];
            octmib_results.relabeling_mode = false;

            // relabel ground truth using this ordering
            std::vector<size_t> reverse_ordering(g.get_num_vertices());
            for (size_t idx1=0; idx1<vertex_subset.size(); idx1++) {
                reverse_ordering[vertex_subset[idx1]] = idx1;
            }

            std::cout << "# CC " << which_cc + 1 << " of ";
            std::cout << vector_of_ccs.size() << std::endl;
            std::cout << "#\tsize: " << vertex_subset.size() << "/";
            std::cout << g.get_num_vertices() << std::endl;

            // Skip Isolated vertices and empty sets
            if (vertex_subset.size() <= 1) {
                octmib_results.isolates ++;
                continue;
            }

            // Isolated edges are MIBs
            if (vertex_subset.size() == 2) {
                BicliqueLite temp((std::vector<size_t>){vertex_subset.front()},
                                  (std::vector<size_t>){vertex_subset.back()});
                octmib_results.push_back_bipartite(temp);
                octmib_results.size_left ++;
                octmib_results.size_right ++;
                continue;
            }

            Graph g_cc = g.subgraph(vertex_subset);

            // Restrict OCT decomposition to this subgraph
            if (input_oct_set.size() > 0) {
                oct_set_cc = input_oct_set.set_intersection(vertex_subset);
                left_set_cc = input_left_set.set_intersection(vertex_subset);
                right_set_cc = input_right_set.set_intersection(vertex_subset);
            }

            // relabel oct decomposition using subgraph labels
            oct_set_cc = convert_node_labels_OVS(oct_set_cc, reverse_ordering);
            left_set_cc = convert_node_labels_OVS(left_set_cc, reverse_ordering);
            right_set_cc = convert_node_labels_OVS(right_set_cc, reverse_ordering);

            octmib_results.turn_on_relabeling_mode(vertex_subset);
            octmib_cc(octmib_results, g_cc, oct_set_cc, left_set_cc, right_set_cc);

            endt = std::clock();
            std::cout << "# this CC ran in ";
            std::cout << double(endt - begint) / CLOCKS_PER_SEC;
            std::cout << "\n#\n# " << std::endl;

        }

    }
    else {
        octmib_cc(octmib_results, g, input_oct_set, input_left_set, input_right_set);
    }

    octmib_results.total_num_mibs += octmib_results.bipartite_num_mibs;
    octmib_results.close_results();

}


/**
 * Wrapper for our OCTMIB algorithm together with OCT decomposition algorithm.
 * This allows user to input a specified OCT decomposition, if desired,
 * but if no oct set is specified, then one is computed before calling the
 * actual OCTMIB algorithm.
 */
void octmib_cc(OutputOptions & octmib_results,
                const Graph & g,
                OrderedVertexSet input_oct_set,
                OrderedVertexSet input_left_set,
                OrderedVertexSet input_right_set) {

    // If we're missing either OCT or left_partition, call simpleOCT
    if (input_oct_set.size() == 0 || input_left_set.size() == 0) {
        clock_t begin = std::clock();
        simpleoct(g, input_oct_set, input_left_set, input_right_set);
        clock_t end = std::clock();
        octmib_results.time_oct_decomp += double(end - begin) / CLOCKS_PER_SEC;

    }
    // If OCT and left_partition are set, then right_partition is determined
    else if (input_right_set.size() == 0) {
        for (size_t idx = 0; idx < g.get_num_vertices(); idx++) {
            if (input_oct_set.has_vertex(idx) || input_left_set.has_vertex(idx)) continue;
            input_right_set.add_vertex_unsafe(idx);
        }
    }

    // Call main OCTMIB function
    std::cout << "#\tOCT in CC has size " << input_oct_set.size() << std::endl;
    std::cout << "#\tL in CC has size " << input_left_set.size() << std::endl;
    std::cout << "#\tR in CC has size " << input_right_set.size() << std::endl;

    // OCT decomposition is determined -- now run OCTMIB
    OctGraph og(g, input_oct_set, input_left_set, input_right_set);

    octmib_results.num_oct_vertices += input_oct_set.size();
    octmib_results.size_left += input_left_set.size();
    octmib_results.size_right += input_right_set.size();

    octmib_with_oct(octmib_results, og);

}

/**
* General-purpose wrapper for the OCTMIB framework; handles disconnected
* components and does not require an OCT-decomposition as input.
*
* Given input graph g, output a vector of all maximal induced bicliques in g.
* If user specifies OCT information, the algorithm will use the decomposition
* provided. Otherwise, this function will call use a simple coloring algorithm
* (via BFS) to create an OCT decomposition.
*/
std::vector<BicliqueLite> octmib(const Graph & g,
                              OrderedVertexSet input_oct_set,
                              OrderedVertexSet input_left_set,
                              OrderedVertexSet input_right_set) {

    OutputOptions octmib_results;

    octmib(octmib_results, g, input_oct_set, input_left_set, input_right_set);

    return octmib_results.mibs_computed;

}
