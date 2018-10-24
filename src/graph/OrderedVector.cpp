/**
 * OrderedVector is a container class that enables efficient set
 * operations by storing the elements sorted in ascending order.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


#include <iostream>
#include <sstream>
#include "OrderedVector.h"


OrderedVector::OrderedVector() {

}


/*
 * Create a new vertex set from a vector of vertices (potentially unordered).
 * WARNING: assumes vector is already sorted in ascending order.
 * Assumes elements appear only once.
 */
OrderedVector::OrderedVector(std::vector<size_t> vertex_list) {

    // Save vertex list
    this->vertex_list = move(vertex_list);

}


/**
 * NOTE: as long as vector is sorted when input,
 * this operation will preserve ordering.
 */
void OrderedVector::vector_intersect_neighborhood( const Graph & graph, size_t vertex) {

    size_t idx_front = 0;
    const std::vector<size_t> & neighbors = graph.get_neighbors_vector(vertex);
    size_t last_neighbor = graph.get_neighbors_vector(vertex).back();

    if (neighbors.size() == 0) {
        vertex_list.clear();
        return;
    }

    for (size_t idx=0; idx < vertex_list.size(); idx++) {
        size_t u = vertex_list[idx];
        if (graph.has_edge(vertex, u)) {
            vertex_list[idx_front]=u;
            idx_front++;
        }
        if (u > last_neighbor) break;
    }
    vertex_list.resize(idx_front);
}


/**
 * NOTE: as long as vector is sorted when input,
 * this operation will preserve ordering.
 */
void OrderedVector::vector_minus_neighborhood( const Graph & graph, size_t vertex) {
    size_t idx_front = 0;
    const std::vector<size_t> & neighbors = graph.get_neighbors_vector(vertex);
    size_t last_neighbor = neighbors.back();

    if (neighbors.size() == 0) return;

    size_t idx = 0;
    for (; idx < vertex_list.size(); idx++) {
        size_t u = vertex_list[idx];
        if (!graph.has_edge(vertex, u)) {
            vertex_list[idx_front]=u;
            idx_front++;
        }
        if (u > last_neighbor) break;
    }
    idx++;
    for (; idx < vertex_list.size(); idx++) {
        vertex_list[idx_front] = vertex_list[idx];
        idx_front++;
    }
    vertex_list.resize(idx_front);

}


/*
 * Adds a vertex, ensures vector remains sorted.
 * WARNING: if this function is passed an element already contained in this,
 * then the element will appear twice in this.
 */
void OrderedVector::bubble_sort(size_t v) {

    // if v goes at end, simply push
    if (vertex_list.size() == 0 || v > vertex_list.back()) {
        vertex_list.push_back(v);
        return;
    }

    // first increase vector by one;
    vertex_list.push_back( v ); // will update value later

    size_t idx = 0;
    while (idx < vertex_list.size()) {
        if (vertex_list[idx] > v) break;
        idx++;
    }
    // if we get to the end, we already inserted correctly.
    if (idx == vertex_list.size()) return;

    // Otherwise, idx is first idx where vertex_list is bigger than v.
    // Gotta insert v and update list after it.
    for (; idx < vertex_list.size(); idx++) {
        size_t temp_val = vertex_list[idx];
        vertex_list[idx] = v;
        v = temp_val;
    }

}


/**
 * Outputs set of vertices as a string separated by commas
 * including an extraneous trailing comma.
 */
std::string OrderedVector::to_string() const {
    std::stringstream s;

    if (vertex_list.size() == 0) return std::string("");

    for (auto v : vertex_list) {
        s << v << ",";
    }

    return s.str();
}


/*
 * OrderedVector::operator== returns true if the input s has the exact same contents as this.
 */
bool OrderedVector::operator==(const OrderedVector &s) const {

    if (size() != s.size()) return false;

    const std::vector<size_t> & s_vertex_list = s.get_vertices();

    return (this->vertex_list == s_vertex_list);

}


/**
 * Compare this OrderedVector to another to determine if they are not equal.
 * Equality is determined by deferring to OrderedVector::operator==.
 *
 * @param  s The OrderedVector that `this` will be compared to.
 * @return   True if the sets are different, false if they are not.
 */
bool OrderedVector::operator!=(const OrderedVector &s) const {
    return !(*this == s);
}


/**
 * Compare this OrderedVector to another to determine if this is
 * lexicographically least.
 * OrderedVector::operator<.
 *
 * @param  s The OrderedVector that `this` will be compared to.
 * @return   True if this < s, false otherwise.
 */
bool OrderedVector::operator<(const OrderedVector &s) const {
    const size_t this_size = vertex_list.size();
    const size_t s_size = s.size();

    if (s_size == 0) return false;  // nothing can be < empty set

    for (size_t idx = 0; idx < this_size; idx++) {
        if (idx >= s_size) return false;  // s is sub set of this -> false
        if (vertex_list[idx] < s[idx]) return true;
        if (vertex_list[idx] > s[idx]) return false;
    }

    if (this_size == s_size) return false; // the sets are equal

    // Reaching this point means this is subset of s, return true.
    return true;
}



/*
 * Given an input vertex v, if v is contained in this, then return a new
 * OrderedVertexSet r which is this set minus all elements of this set
 * less than v. If v not in this, return r empty.
 */
std::vector<size_t> OrderedVector::truncate_before_node_vec(size_t v) const {

    // if v not contained, return empty vector
    if (!has_vertex(v)) return std::vector<size_t>{};

    // else find index of v
    size_t index_of_v;
    for (size_t idx=0; idx<size(); idx++) {
        if (vertex_list[idx] == v) {
            index_of_v = idx;
            break;
        };
    }

    // create vector from everything after v
    std::vector<size_t> vertices;
    for (size_t idx = index_of_v+1; idx<size(); idx++) {
        vertices.push_back(vertex_list[idx]);
    }

    return vertices;

}


/*
 * Given an input vertex v, if v is contained in this, then return a new
 * OrderedVertexSet r which is all elements of this set less than v.
 * If v not in this, return r empty.
 */
std::vector<size_t> OrderedVector::truncate_after_node_vec(size_t v) const {

    // if v not contained, return empty vector
    if (!has_vertex(v)) return std::vector<size_t>{};

    // create vector from everything before v
    std::vector<size_t> vertices;
    for (auto node : this->vertex_list) {
        if (node >= v) break;
        vertices.push_back(node);
    }

    return vertices;

}
