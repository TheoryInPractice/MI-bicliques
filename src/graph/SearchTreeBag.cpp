/**
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


// Include SearchTreeBag header
#include "SearchTreeBag.h"


/**
 * Check if a blueprint with `blueprint_ID` is in the bag. If yes,
 * Ban vertex_to_ban in that blueprint, and return True.
 * Else, return False.
 *
 * @param vertex_to_ban     vertex to ban inside specified blueprint
 * @param blueprint_ID      ID to check for in the bag.
 *
 * @return bool             whether or not blueprint_ID is in bag.
 */
bool SearchTreeBag::ban_vertex_in_blueprints(const size_t & vertex_to_ban,
                                             const size_t & blueprint_ID) {

    // if the ID isn't present, skip
    if (blueprint_ID_to_bag_index.find(blueprint_ID) == blueprint_ID_to_bag_index.end()) {
        return false;
    }
    // else, the ID is present, so ban
    bag[blueprint_ID_to_bag_index[blueprint_ID]].ban_vertex(vertex_to_ban);
    return true;
}


/**
 * Add input `blueprint` to this SearchTreeBag object, and update
 * the ID-to-blueprint-index map accordingly.
 *
 * @param blueprint         A blueprint to add to the bag.
 */
void SearchTreeBag::add_blueprint(BlueprintLite & blueprint) {

    blueprint_ID_to_bag_index[blueprint.get_id()] = bag.size();
    bag.push_back(blueprint);
}


/**
 * Construct a new empty SearchTreeBag with empty `bag` and `blueprint_ID_to_bag_index`.
 */
SearchTreeBag::SearchTreeBag() {

}


/**
 * Construct a new empty SearchTreeBag with expanded_vertex specified.
 */
SearchTreeBag::SearchTreeBag(size_t expanded_vertex) : SearchTreeBag() {

    this->bag = (std::vector<BlueprintLite>) {};
    this->blueprint_ID_to_bag_index = (std::unordered_map<size_t,size_t>) {};
    this->expanded_vertex = expanded_vertex;

}


/**
 * Construct a new empty SearchTreeBag with expanded_vertex specified.
 */
SearchTreeBag::SearchTreeBag(std::vector<size_t> I_wait,
                             OrderedVertexSet I_in,
                             std::vector<size_t> I_past,
                             size_t expanded_vertex) : SearchTreeBag(expanded_vertex) {

    this->iterative_in = I_in;
    this->iterative_past = I_past;
    this->iterative_wait = I_wait;

    this->iterative_wait_ptr = std::make_shared<std::vector<size_t>>(iterative_wait);
    this->iterative_in_ptr = std::make_shared<OrderedVertexSet>(iterative_in);
    this->iterative_past_ptr = std::make_shared<std::vector<size_t>>(iterative_past);
}


void SearchTreeBag::expand_on_vertex(size_t expanded_vertex) {
    // Add v to iterative in, update iterative_past and iterative_wait
    iterative_in.add_vertex_unsafe(expanded_vertex);
    for (auto u : iterative_wait) {
        if (u < expanded_vertex ) {
            if (!iterative_in.has_vertex(u)) iterative_past.push_back(u);
            // It's safe to use push_back here because iterative_wait
            // vertices must all be greater than iterative_past.
        }
        if (u >= expanded_vertex) break;
    }

    // Erase everything in iterative_wait up to and including expanded vertex
    size_t index_of_v;
    for (auto idx=0; idx < iterative_wait.size(); idx++) {
        if (iterative_wait[idx] == expanded_vertex) {
            index_of_v = idx;
            break;
        }
        if (iterative_wait[idx] > expanded_vertex) {
            index_of_v = iterative_wait.size();
            break;
        }
    }
    iterative_wait.erase(iterative_wait.begin(), iterative_wait.begin() + index_of_v + 1);

    iterative_wait_ptr = std::make_shared<std::vector<size_t>>(iterative_wait);
    iterative_in_ptr = std::make_shared<OrderedVertexSet>(iterative_in);
    iterative_past_ptr = std::make_shared<std::vector<size_t>>(iterative_past);

    this->expanded_vertex = expanded_vertex;
}
