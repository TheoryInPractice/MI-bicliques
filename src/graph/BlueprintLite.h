/**
 * BlueprintLite is a builder for Bicliques. See BlueprintLite.cpp for full documentation.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


#ifndef BICLIQUES_BLUEPRINTLITE_H
#define BICLIQUES_BLUEPRINTLITE_H


#include <memory> // for std::shared_ptr
#include <cstddef>
#include <string>
#include <tuple>
#include "BicliqueLite.h"
#include "ExpansionResult.h"
#include "Graph.h"


class BlueprintLite {

private:
    size_t next_future_maximal;
    size_t id;
    std::shared_ptr<Graph> graph;
    std::shared_ptr<OrderedVertexSet> iter_in;
    std::shared_ptr<std::vector<size_t>> iter_wait, iter_past;
    std::vector<size_t>
            completely_connected_in,
            completely_connected_out,
            independent_from_in,
            independent_from_out;

    std::vector<size_t> oct_auxiliary_independent_set;

    std::unordered_map<size_t, bool> ban_list;

    void vector_intersect_neighborhood(std::vector<size_t> & , size_t );
    void vector_minus_neighborhood(std::vector<size_t> & , size_t );
    std::vector<size_t> vector_union(const std::vector<size_t> &,
                                     const std::vector<size_t> & ) const;

public:

    BlueprintLite(size_t id,
                  std::shared_ptr<Graph> graph,
                  std::shared_ptr<std::vector<size_t>> iterative_wait,
                  std::shared_ptr<OrderedVertexSet> iterative_in,
                  std::shared_ptr<std::vector<size_t>> iterative_past,
                  std::vector<size_t> completely_connected_in,
                  std::vector<size_t> completely_connected_out,
                  std::vector<size_t> independent_from_in,
                  std::vector<size_t> independent_from_out,
                  std::vector<size_t> oct_auxiliary_independent_set
                 );

    ExpansionResult expand(const size_t v,
                           const std::unordered_map<std::string,bool> & duplicates_list);

    void update_iterative_ptrs(std::shared_ptr<std::vector<size_t>> iter_wait,
                               std::shared_ptr<OrderedVertexSet> iter_in,
                               std::shared_ptr<std::vector<size_t>> iter_past);

    void print_iteratives() {
        std::cout << "\tInside BlueprintLite" << std::endl;
        std::cout << "\t iterative_wait is " << vector_to_string(*iter_wait) << std::endl;
        std::cout << "\t iterative_in is " << iter_in->to_string() << std::endl;
        std::cout << "\t iterative_past is " << vector_to_string(*iter_past) << std::endl;
    };

    void print_non_iteratives() const {
        std::cout << "\tInside BlueprintLite" << std::endl;
        std::cout << "\t cc_in is " << vector_to_string(completely_connected_in) << std::endl;
        std::cout << "\t cc_out is " << vector_to_string(completely_connected_out) << std::endl;
        std::cout << "\t if_in is " << vector_to_string(independent_from_in) << std::endl;
        std::cout << "\t if_out is " << vector_to_string(independent_from_out) << std::endl;
    };

    BicliqueLite get_biclique();
    std::string get_biclique_string();
    BicliqueLite get_partial_biclique();
    std::string get_partial_biclique_string();

    inline const auto & get_completely_connected_in() const {
        return completely_connected_in;
    };

    bool is_banned(const size_t v) const;
    bool ban_vertex(const size_t v);

    bool is_currently_maximal();
    bool is_currently_maximal(size_t & next_future_max_vert);

    inline size_t get_id() {
        return id;
    }

    /**
     * Check to see if some set of vertices can be added to the left side of the current Biclique. This is true if and only
     * if no vertex in the set is completely connected to the set `completely_connected_in` and is completely independent
     * from `independent_from_in`.
     *
     * @param  s Set of vertices to check to be added to the left side of the Biclique.
     * @return   True if any vertex can be added, false if not.
     */
    template<class T>
    bool can_be_added_left(const T &s,
                           size_t & next_future_maximal_vertex) const {
        // empty set can trivially be added, but is irrelevant, return false
        if (s.size() == 0) return false;

        for (auto v : s) {
            if (graph->is_completely_connect_to(v, completely_connected_in) &&
                graph->is_completely_independent_from(v, independent_from_in)) {

                next_future_maximal_vertex = v;
                return true;
            }
        }
        return false;
    }
    // Overload function so it can be called without size_t parameter
    template<class T>
    inline bool can_be_added_left(const T &s) const {
        size_t dummy;
        return can_be_added_left(s, dummy);
    }

    template<class T>
    bool can_be_added_right(const T &s) const;


    inline size_t get_next_future_maximal() const {
        return this->next_future_maximal;
    }

    inline bool set_next_future_maximal(const size_t nfmv) {
        if (nfmv <= get_last_vertex()) {
            this->next_future_maximal = nfmv;
            return true;
        }
        return false;
    }

    inline size_t get_last_vertex() {
        return graph->get_num_vertices();
    }

    bool skip_expanding_vertex(const size_t u) const;
};

#endif //BICLIQUES_BLUEPRINTLITE_H
