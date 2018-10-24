/**
 * OrderedVertexSet is a container class for efficient set operations.
 * Must be initialized with vertex list in ascending order.
 *
 * See OrderedVertexSet.cpp for full documentation.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

#ifndef BICLIQUES_ORDEREDVECTOR_H
#define BICLIQUES_ORDEREDVECTOR_H


#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>

#include "Graph.h"

class OrderedVector {

private:
    std::vector<size_t> vertex_list;

public:
    OrderedVector();
    OrderedVector(std::vector<size_t> vertex_list);

     // Return reference to the vector vertex_list.
    inline const std::vector<size_t>& get_vertices() const {
        return vertex_list;
    }

    // CUSTOM OPERATIONS
    /*
     * Given another OrderedVector s, return a new OrderedVector r which is
     * the union of this set and s.
     * NOTE: as long as vector is sorted when input,
     * this operation will preserve ordering.
     */
    template <typename T>
    std::vector<size_t> set_union(const T &vector1) const {

         std::vector<size_t> dummy(vector1.size() + vertex_list.size());

         auto it = std::set_union (vector1.begin(), vector1.end(), vertex_list.begin(), vertex_list.end(), dummy.begin());

         dummy.resize(it-dummy.begin());

         return dummy;

    }

    /*
     * Given another OrderedVector s, return a new OrderedVector r which is
     * s.set_minus(r)
     * NOTE: as long as vector is sorted when input,
     * this operation will preserve ordering.
     */
    template <typename T>
    std::vector<size_t> set_minus(const T &vector1) const {

         std::vector<size_t> dummy(vertex_list.size());

         auto it = std::set_difference (vertex_list.begin(), vertex_list.end(), vector1.begin(), vector1.end(), dummy.begin());

         dummy.resize(it-dummy.begin());

         return dummy;

    }

    /*
     * Given another OrderedVector s, return a new OrderedVector r which is
     * s.set_intersect(r)
     * NOTE: as long as vector is sorted when input,
     * this operation will preserve ordering.
     */
    template <typename T>
    std::vector<size_t> set_intersection(const T &vector1) const {

         std::vector<size_t> dummy(vertex_list.size());
         auto it = std::set_intersection (vertex_list.begin(), vertex_list.end(), vector1.begin(), vector1.end(), dummy.begin());
         dummy.resize(it-dummy.begin());

         return dummy;
    }

    void vector_intersect_neighborhood( const Graph &, size_t);
    void vector_minus_neighborhood( const Graph &, size_t);

    void bubble_sort(size_t v);
    inline void push_back(size_t v) { vertex_list.push_back(v); }
    inline bool has_vertex(const size_t v_target) const {
        // binary search to determine if an input integer is contained in this.
        return std::binary_search(vertex_list.begin(),
                                  vertex_list.end(),
                                  v_target);
    }


    std::vector<size_t> truncate_before_node_vec(size_t v) const;
    std::vector<size_t> truncate_after_node_vec(size_t v) const;

    std::string to_string() const;

    // STANDARD OPERATIONS
    bool operator==(const OrderedVector &other) const;
    bool operator!=(const OrderedVector &other) const;
    bool operator<(const OrderedVector &other) const;

    inline void clear() { vertex_list.clear(); }

    inline size_t size() const { return vertex_list.size(); }
    inline size_t operator[] (const size_t idx) const { return vertex_list[idx]; }

    inline size_t front() const { return vertex_list.front(); }
    inline size_t back() const { return vertex_list.back(); }

    // define pieces for iterator
    inline std::vector<size_t>::iterator begin() {return vertex_list.begin();}
    inline std::vector<size_t>::iterator end() {return vertex_list.end();}
    inline std::vector<size_t>::const_iterator begin() const {return vertex_list.begin();}
    inline std::vector<size_t>::const_iterator end() const {return vertex_list.end();}

};


#endif //BICLIQUES_ORDEREDVECTOR_H
