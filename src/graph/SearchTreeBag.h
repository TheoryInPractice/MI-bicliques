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

#ifndef BICLIQUES_SEARCHTREEBAG_H
#define BICLIQUES_SEARCHTREEBAG_H

#include <vector>
#include "BlueprintLite.h"
#include "OrderedVertexSet.h"


class SearchTreeBag {

public:
    SearchTreeBag();
    SearchTreeBag(size_t expanded_vertex);
    SearchTreeBag(std::vector<size_t> I_wait,
                  OrderedVertexSet I_in,
                  std::vector<size_t> I_past,
                  size_t expanded_vertex);

    void add_blueprint(BlueprintLite & blueprint);
    void expand_on_vertex(size_t expanded_vertex);

    bool ban_vertex_in_blueprints(const size_t & vertex_to_ban,
                                  const size_t & blueprint_ID);

    inline size_t size() const { return bag.size(); }

    inline const BlueprintLite & front() const { return bag.front(); }
    inline const BlueprintLite & back() const { return bag.back(); }
    inline BlueprintLite & back() { return bag.back(); }
    inline void pop_back() { bag.pop_back(); }

    inline void clear() {
        iterative_in.clear();
        iterative_wait.clear();
        iterative_past.clear();
        bag.clear();
        blueprint_ID_to_bag_index.clear();
    }

    // define pieces for iterator
    inline std::vector<BlueprintLite>::iterator begin() {return bag.begin();}
    inline std::vector<BlueprintLite>::iterator end() {return bag.end();}

    inline std::vector<BlueprintLite>::const_iterator begin() const {return bag.begin();};
    inline std::vector<BlueprintLite>::const_iterator end() const {return bag.end();};

    inline size_t get_expanded_vertex() const {
        return this->expanded_vertex;
    }

    inline OrderedVertexSet& get_iterative_in() {
        return this->iterative_in;
    }

    inline std::vector<size_t>& get_iterative_wait() {
        return this->iterative_wait;
    }

    inline std::vector<size_t>& get_iterative_past() {
        return this->iterative_past;
    }

    inline std::shared_ptr<std::vector<size_t>> & get_iterative_wait_ptr() {
        return iterative_wait_ptr;
    }

    inline std::shared_ptr<OrderedVertexSet> & get_iterative_in_ptr() {
        return iterative_in_ptr;
    }

    inline std::shared_ptr<std::vector<size_t>> & get_iterative_past_ptr() {
        return iterative_past_ptr;
    }

    inline size_t get_last_iter_in_node() const {
        return iterative_in.back();
    }

private:
    std::shared_ptr<OrderedVertexSet> iterative_in_ptr;
    std::shared_ptr<std::vector<size_t>> iterative_wait_ptr, iterative_past_ptr;

    OrderedVertexSet iterative_in;
    std::vector<size_t> iterative_wait, iterative_past;

    std::vector<BlueprintLite> bag;
    std::unordered_map<size_t,size_t> blueprint_ID_to_bag_index;
    size_t expanded_vertex;

};


#endif //BICLIQUES_SEARCHTREEBAG_H
