/**
 * ExpansionResult bundles metadata about the outcome of calling BlueprintLite::expand. See BlueprintLite.cpp
 * for full documentation.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

#ifndef BICLIQUES_SEARCHTREEBAGPLUS_H
#define BICLIQUES_SEARCHTREEBAGPLUS_H

#include "BicliqueLite.h"
#include "Graph.h"
#include <algorithm> // for std::sort
#include <memory> // for std::shared_ptr
#include "stddef.h"  // apparently required for size_t

class SearchTreeBagPlus {

public:
    SearchTreeBagPlus();
    SearchTreeBagPlus(size_t expanded_vertex);
    SearchTreeBagPlus(std::shared_ptr<Graph> graph_ptr,
                      std::vector<size_t> CC_in,
                      std::vector<size_t> I_wait,
                      std::vector<size_t> I_in,
                      std::vector<size_t> I_past,
                      size_t expanded_vertex);

    bool expand(const size_t expanded_vertex);

    // CC_in is always sorted; iterative_in might not be
    inline BicliqueLite get_biclique(){
        return BicliqueLite(iterative_in, completely_connected_in);
    }

    inline const std::vector<size_t> & get_iterative_wait() const {
        return iterative_wait;
    }

    inline size_t get_expanded_vertex() { return expanded_vertex; }


private:

    /**
     * Check to see if some set of vertices can be added to the left side of
     * the current Biclique. This is true if and only if no vertex in the set is
     * completely connected to the set `completely_connected_in` and is
     * completely independent from `independent_from_in` (which is always empty
     * in this structure, so omitted).
     *
     * @param  s Set of vertices to check to be added to the left side of the Biclique.
     * @return   True if any vertex can be added, false if not.
     */
    inline bool can_be_added_left(const std::vector<size_t> &s) const {
        // empty set can trivially be added, but is irrelevant, return false
        if (s.size() == 0) return false;
        for (const size_t v : s) {
            if (graph_ptr->is_completely_connect_to(v, completely_connected_in)) {
                return true;
            }
        }
        return false;
    }

    std::vector<size_t> iterative_in, iterative_wait, iterative_past;
    std::vector<size_t> completely_connected_in;

    std::shared_ptr<Graph> graph_ptr;

    size_t expanded_vertex;

};


#endif //BICLIQUES_SEARCHTREEBAGPLUS_H
