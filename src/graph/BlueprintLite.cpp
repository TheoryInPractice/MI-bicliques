/**
 * BlueprintLite is a builder for Bicliques. It represents some Biclique built
 * by continually expanding the blueprint. A blueprint can be checked for the
 * maximality of its underlying Biclique. When ready, the Biclique may be built
 * from the blueprint.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

// Include BlueprintLite header
#include "BlueprintLite.h"



/**
 * Construct a new BicliqueBlueprint.
 *
 * @param id                            An identifier for the BicliqueBlueprint used for tracking related blueprints.
 * @param graph                         Pointer to the graph instance Bicliques live in.
 * @param iterative_in                  Vertices which are currently in the Biclique.
 * @param iterative_past                Vertices which have already been under consideration for being in the Biclique.
 * @param iterative_wait                Vertices which have not yet been expanded upon.
 * @param completely_connected_in       Vertices in the Biclique which are completely connected to iterative_in.
 * @param completely_connected_out      Vertices not in the Biclique which are completely connected to iterative_in.
 * @param independent_from_in           Vertices in the Biclique which are completely independent from iterative_in.
 * @param independent_from_out          Vertices not in the Biclique which are completely independent from iterative_in.
 * @param oct_auxiliary_independent_set
 */
BlueprintLite::BlueprintLite(size_t id,
                             std::shared_ptr<Graph> graph,
                             std::shared_ptr<std::vector<size_t>> iterative_wait,
                             std::shared_ptr<OrderedVertexSet> iterative_in,
                             std::shared_ptr<std::vector<size_t>> iterative_past,
                             std::vector<size_t> completely_connected_in,
                             std::vector<size_t> completely_connected_out,
                             std::vector<size_t> independent_from_in,
                             std::vector<size_t> independent_from_out,
                             std::vector<size_t> oct_auxiliary_independent_set
                             ) :
                             id(id),
                             graph(graph),
                             iter_wait(iterative_wait),
                             iter_in(iterative_in),
                             iter_past(iterative_past),
                             completely_connected_in(completely_connected_in),
                             completely_connected_out(completely_connected_out),
                             independent_from_in(independent_from_in),
                             independent_from_out(independent_from_out),
                             oct_auxiliary_independent_set(oct_auxiliary_independent_set){

    this->next_future_maximal = this->get_last_vertex();

}


/**
 * Construct a new Biclique instance from the information currently in the
 * BicliqueBlueprint. The left side of the Biclique is formed from the union
 * of `iterative_in` and `independent_from_in`. The right side of the Biclique
 * is formed from `completely_connected_in`.
 *
 * @return Biclique built from the current state of the BicliqueBlueprint.
 */
BicliqueLite BlueprintLite::get_biclique() {

    return BicliqueLite( vector_union(iter_in->get_vertices(), independent_from_in), completely_connected_in);

}


/**
 * Get the string representation of the Biclique which can be built by the
 * current Blueprint.
 *
 * @return Current Biclique's string representation.
 */
std::string BlueprintLite::get_biclique_string() {

    std::vector<size_t> temp = vector_union(iter_in->get_vertices(), independent_from_in);
    std::vector<size_t> dummy = vector_union(temp, completely_connected_in);
    return vector_to_string(dummy);

}


/**
 * Return a partial Biclique constructed from `independent_from_in` and
 * `completely_connected_in` as the left and right sides, respectively.
 * Getting a partial biclique is useful in checking for duplication, where
 * every Biclique shares `iterative_in`, meaning vertices in `iterative_in`
 * can be removed from consideration as an optimization step.
 *
 * @return Partial Biclique not containing vertices from `iterative_in`.
 */
BicliqueLite BlueprintLite::get_partial_biclique() {

    return BicliqueLite(independent_from_in, completely_connected_in);

}


/**
 * Get the string representation of the partial Biclique which can be built by
 * the current Blueprint.
 *
 * @return Current partial Biclique's string representation.
 */
std::string BlueprintLite::get_partial_biclique_string() {

    auto dummy = vector_union(independent_from_in, completely_connected_in);
    return vector_to_string(dummy);
}


/**
 * Check to see if some set of vertices can be added to the right side of the
 * current Biclique. This is false if and only no vertex in the set is
 * completely connected to the set `independent_from_in` and is completely
 * independent from `completely_connected_in`.
 *
 * @param  s Set of vertices to check to add to the right side of the Biclique.
 * @return   True if any vertex can be added, false if not.
 */
template<class T>
bool BlueprintLite::can_be_added_right(const T &s) const {

    // empty set can trivially be added, but is irrelevant, return false
    if (s.size() == 0) return false;

    for (auto v : s) {

        if (graph->is_completely_connect_to(v, independent_from_in) &&
            graph->is_completely_independent_from(v, completely_connected_in)) {

            return true;

        }

    }

    return false;

}


/**
 * Check if the Biclique represented by BicliqueBlueprint is currently maximal.
 * This is true if no vertex from `iterative_wait` can be added to the left,
 * and no vertex from oct_auxiliary_independent_set can be added to the left.
 *
 * @return True if the Biclique is currently maximal.
 */
bool BlueprintLite::is_currently_maximal() {

    return (!can_be_added_left(*iter_wait) && !can_be_added_left(oct_auxiliary_independent_set));

}
/**
 * Overload funtion is_currently_maximal() so that we can get next future
 * maximal vertex in the case that is_currently_maximal outputs false.
 */
bool BlueprintLite::is_currently_maximal(size_t & next_future_max_vert) {

    return (!can_be_added_left(*iter_wait, next_future_max_vert) && !can_be_added_left(oct_auxiliary_independent_set));

}


/**
 * Check to see if a vertex has been banned in the BicliqueBlueprint.
 * This defaults to false if not in the map of banned vertices.
 * If there is a status for v in the map, it is returned.
 *
 * @param  v Vertex to return a banned status for.
 * @return   True if `v` is currently banned, false if not.
 */
bool BlueprintLite::is_banned(const size_t v) const {

    if (ban_list.find(v) != ban_list.end()) {
        return true;  // assumes ban_list[v] is true iff v in ban_list
    }
    return false;

}


/**
 * Add a vertex to a BicliqueBlueprint's ban_list. This returns false if
 * the vertex is already in the ban_list.
 * @param  v Vertex to add to the ban_list.
 * @return   True if `v` was not already banned, false if it was.
 */
bool BlueprintLite::ban_vertex(const size_t v) {
    ban_list[v] = true;
    return true;
}


/**
 * Expand accepts a vertex v and a previously generated Biclique_Blueprint.
 * V is added to iterative_in, and the remaining sets are updated to remain
 * consistent with the change to iterative_in.
 * WARNING: changes to the iterative sets happen externally, in SearchTreeBag
 *
 *
 * Expansion Procedure
 * *******************
 *
 * Expand on a vertex v by:
 * - Check for short circuiting conditions
 * - Intersect N(v) with completely_connected_in and completely_connected_out
 * - Set minus N(v) from independent_from_in and independent_from_out
 * - Check for failure conditions
 * - Add v to iterative_in, update iterative_past, iterative_wait, return success
 *
 *
 * Short Circuiting Conditions
 * ***************************
 *
 * If v is in the ban list, return an ExpansionResult that indicates expansion
 * should be discontinued.
 *
 * Additionally we check if the expanded with node is greater than the
 * next_future_maximal node before calling expand, which essentially serves
 * as a short circuiting condition in the case where it is.
 *
 *
 * Failure Conditions and Return Values
 * ************************************
 *
 * We expand the "expanded-from" blueprint to create the "expanded-to" blueprint.
 *
 * If completely_connected_in is empty after intersecting with N(v), return an
 * ExpansionResult indicating expansion should be discontinued and v
 * should be banned in all blueprints created by expanding the "expanded-from"
 * blueprint with a node from wait_front (the nodes from iterative_wait which are less than v).
 *
 * Duplication termination criterion:
 * If, after expansion, the biclique in the "expanded-to" blueprint is identical to one of the bicliques
 * in a previously expanded blueprint, discontinue the expansion.

 *
 * Future-maximality conditions:
 * - If a vertex u from iterative_past passes the check can_be_added_left, return an ExpansionResult indicating
 * that expansion should be discontinued and v will be banned in all blueprints created by expanding the "expanded-from"
 * blueprint with a node from wait_front.
 * - If a vertex u from  wait_front passes the check can_be_added_left for the "expanded-to" blueprint,
 * then discontinue the expansion.
 *
 * Non-relevancy conditions:
 * - If a vertex u from independent_from_out passes the check can_be_added_left or a vertex w
 * from completely_connected_from_out passes the check can_be_added_right,
 * then discontinue the expansion.
 */
ExpansionResult BlueprintLite::expand(const size_t expanded_vertex,
                                      const std::unordered_map<std::string, bool> & duplicates_list
                                  ) {

    //WARNING: the updates to the iterative sets happen externally, in
    // searchtreebag. This is called *before* the iterative sets are udpated.

    // If expanded_vertex is banned, discontinue
    if (is_banned(expanded_vertex)) {
        return ExpansionResult(ExpansionResult::RETURN_TYPE::DISCONTINUE);
    }

    // Intersect neighborhood of expanded_vertex with blueprint sets
    const OrderedVertexSet & neighbors = graph->get_neighbors(expanded_vertex);
    vector_intersect_neighborhood(completely_connected_in, expanded_vertex);
    vector_intersect_neighborhood(completely_connected_out, expanded_vertex);
    vector_minus_neighborhood(independent_from_in, expanded_vertex);
    vector_minus_neighborhood(independent_from_out, expanded_vertex);

    // Start checking for failure conditions
    if (completely_connected_in.size()==0) {
        return ExpansionResult(ExpansionResult::RETURN_TYPE::BAN, expanded_vertex);
    }

    // Check for duplication
    if (duplicates_list.find(get_partial_biclique_string()) != duplicates_list.end()) {
        return ExpansionResult(ExpansionResult::RETURN_TYPE::DISCONTINUE);
    }

    // Non-relevancy check
    if (can_be_added_left(independent_from_out) || can_be_added_right(completely_connected_out)) {
        return ExpansionResult(ExpansionResult::RETURN_TYPE::DISCONTINUE);
    }

    // Future maximal check
    if (can_be_added_left(*iter_past)) {
        return ExpansionResult(ExpansionResult::RETURN_TYPE::BAN, expanded_vertex);
    }

    // If node in iter wait can be added, then not future maximal
    for (size_t u : *iter_wait) {
        if (u < expanded_vertex &&
            graph->is_completely_connect_to(u, completely_connected_in) &&
            graph->is_completely_independent_from(u, independent_from_in)) {

            return ExpansionResult(ExpansionResult::RETURN_TYPE::DISCONTINUE);
        }
        if (u >= expanded_vertex) break;
    }

    vector_minus_neighborhood(oct_auxiliary_independent_set, expanded_vertex);

    return ExpansionResult(ExpansionResult::RETURN_TYPE::SUCCESS);

}


bool BlueprintLite::skip_expanding_vertex(const size_t u) const {
    // If u is beyond the next node needed for maximality, skip
    if (u > get_next_future_maximal()) {
        return true;
    }
    // If u is banned in blueprint, skip
    if (is_banned(u)) {
        return true;
    }
    return false;
}


void BlueprintLite::update_iterative_ptrs(std::shared_ptr<std::vector<size_t>> iter_wait,
                                           std::shared_ptr<OrderedVertexSet> iter_in,
                                           std::shared_ptr<std::vector<size_t>> iter_past) {

    this->iter_wait = iter_wait;
    this->iter_in = iter_in;
    this->iter_past = iter_past;
}


/**
 * NOTE: as long as vector is sorted when input,
 * this operation will preserve ordering.
 */
void BlueprintLite::vector_intersect_neighborhood(std::vector<size_t> & vector, size_t vertex) {

    size_t idx_front = 0;
    const std::vector<size_t> & neighbors = graph->get_neighbors_vector(vertex);

    if (neighbors.size() == 0) {
        vector.clear();
        return;
    }
    size_t last_neighbor = neighbors.back();

    for (size_t idx=0; idx < vector.size(); idx++) {
        size_t u = vector[idx];
        if (graph->has_edge(vertex, u)) {
            vector[idx_front]=u;
            idx_front++;
        }
        if (u > last_neighbor) break;
    }
    vector.resize(idx_front);
}


/**
 * NOTE: as long as vector is sorted when input,
 * this operation will preserve ordering.
 */
void BlueprintLite::vector_minus_neighborhood(std::vector<size_t> & vector, size_t vertex) {
    size_t idx_front = 0;
    const std::vector<size_t> & neighbors = graph->get_neighbors_vector(vertex);

    if (neighbors.size() == 0) return;
    size_t last_neighbor = neighbors.back();

    size_t idx = 0;
    for (; idx < vector.size(); idx++) {
        size_t u = vector[idx];
        if (!graph->has_edge(vertex, u)) {
            vector[idx_front]=u;
            idx_front++;
        }
        if (u > last_neighbor) break;
    }
    idx++;
    for (; idx < vector.size(); idx++) {
        vector[idx_front] = vector[idx];
        idx_front++;
    }
    vector.resize(idx_front);

}


/**
 * NOTE: as long as vector is sorted when input,
 * this operation will preserve ordering.
 */
std::vector<size_t> BlueprintLite::vector_union(const std::vector<size_t> & vector1, const std::vector<size_t> & vector2) const {

    std::vector<size_t> dummy(vector1.size() + vector2.size());

    auto it = std::set_union (vector1.begin(), vector1.end(), vector2.begin(), vector2.end(), dummy.begin());

    dummy.resize(it-dummy.begin());

    return dummy;

}
