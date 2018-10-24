/**
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


// Include SearchTreeBagPlus header
#include "SearchTreeBagPlus.h"


/**
 * Construct a new empty SearchTreeBagPlus.
 */
SearchTreeBagPlus::SearchTreeBagPlus() {

}


/**
 * Construct a new empty SearchTreeBagPlus with expanded_vertex specified.
 */
SearchTreeBagPlus::SearchTreeBagPlus(size_t expanded_vertex) : SearchTreeBagPlus() {

    this->expanded_vertex = expanded_vertex;

}


/**
 * Construct a new SearchTreeBagPlus with all necessary pieces.
 */
SearchTreeBagPlus::SearchTreeBagPlus(std::shared_ptr<Graph> graph_ptr,
                                     std::vector<size_t> CC_in,
                                     std::vector<size_t> I_wait,
                                     std::vector<size_t> I_in,
                                     std::vector<size_t> I_past,
                                     size_t expanded_vertex)
                                     :
                                     graph_ptr(graph_ptr),
                                     completely_connected_in(CC_in),
                                     iterative_wait(I_wait),
                                     iterative_in(I_in),
                                     iterative_past(I_past),
                                     expanded_vertex(expanded_vertex){
    // do nothing else

}


/**
 * Expand accepts a vertex, expanded_vertex, which is added to iterative_in;
 * the remaining sets are updated to remain consistent with the change to
 * iterative_in, and the updated object is checked to see if it's a maximal
 * induced biclique. See BlueprintLite::expand() for a more detailed
 * explanation of a very similar function.
 */
bool SearchTreeBagPlus::expand(const size_t expanded_vertex) {

    // Get neighbors of expanded_vertex
    const OrderedVertexSet & neighbors = graph_ptr->get_neighbors(expanded_vertex);
    const size_t last_neighbor = neighbors.back();

    // intersect completely_connected_in with neighbors
    size_t idx_front = 0;
    for (size_t idx=0; idx < completely_connected_in.size(); idx++) {
        size_t u = completely_connected_in[idx];
        if (graph_ptr->has_edge(expanded_vertex, u)) {
            completely_connected_in[idx_front]=u;
            idx_front++;
        }
        if (u > last_neighbor) break;
    }
    completely_connected_in.resize(idx_front);

    // If CC_in is empty, this can never be expanded into a biclique; STOP
    if (completely_connected_in.size()==0) {
        return false;
    }

    // If any node in iter_past can be added left, this biclique found elsewhere
    if (can_be_added_left(iterative_past)) {
        return false;
    }

    // If node in iter wait can be added, then not future maximal
    for (const size_t u : iterative_wait) {
        if (u < expanded_vertex &&
            graph_ptr->is_completely_connect_to(u, completely_connected_in)) {
            return false;
        }
        if (u >= expanded_vertex) break;
    }

    // IF WE PASS THIS POINT --- SUCCESSFUL EXPAND: update iterative sets

    // Add expanded_vertex to iterative in, update iterative_past
    std::vector<size_t> merge_with_iter_in;
    merge_with_iter_in.push_back(expanded_vertex);
    size_t index_of_v = 0;
    for (const size_t u : iterative_wait) {
        if (u < expanded_vertex ) {
            iterative_past.push_back(u);
            index_of_v++;
        }
        if (u == expanded_vertex) break;
    }

    // Add to iterative_in everything from iterative_wait that is
    // completely connected to CC_in, then update iterative_in
    idx_front = 0;
    for (size_t idx = index_of_v+1; idx < iterative_wait.size(); idx++) {
        size_t u = iterative_wait[idx];
        if (graph_ptr->is_completely_connect_to(u, completely_connected_in)) {
            merge_with_iter_in.push_back(u);
        }
        else {
            iterative_wait[idx_front] = u;
            idx_front++;
        }
    }
    iterative_wait.resize(idx_front);

    size_t n = iterative_in.size();
    iterative_in.insert(iterative_in.end(), merge_with_iter_in.begin(), merge_with_iter_in.end());
    std::inplace_merge(iterative_in.begin(), iterative_in.begin() + n, iterative_in.end());


    this->expanded_vertex = expanded_vertex;

    return true;
}
