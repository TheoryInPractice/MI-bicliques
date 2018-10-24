/**
 * OrderedVertexSet is a container class that enables efficient set
 * operations by storing the elements of a set as both a hashtable, for
 * efficient element containment checks, as well as a sorted vector, for
 * efficient operations like set minus and checking whether one set is a subset
 * of another.
 * An OrderedVertexSet can be created from a hashtable or an (ordered!) vector.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


#include <iostream>
#include <sstream>
#include "OrderedVertexSet.h"


OrderedVertexSet::OrderedVertexSet() {

}


/*
 * Create a new vertex set from a vector of vertices (potentially unordered).
 */
OrderedVertexSet::OrderedVertexSet(std::vector<size_t> vertex_list) {

    // Save vertex list
    this->vertex_list = move(vertex_list);

    // Init vertex map
    for (auto v : this->vertex_list) {
        vertex_map[v] = true;
    }

}

/*
 * Create a new vertex set from an unordered map of vertices (definitely unordered).
 */
OrderedVertexSet::OrderedVertexSet(std::unordered_map<size_t, bool> vertex_map) {

    // Init vertex list
    for (auto kv : vertex_map) {

        if (vertex_map[kv.first]) vertex_list.push_back(kv.first);

    }

    std::sort(vertex_list.begin(), vertex_list.end());

    // Save vertex map
    this->vertex_map = move(vertex_map);

}


/*
 * Given another OrderedVertexSet s, return a new OrderedVertexSet r which is the union
 * of this set and s.
 */
OrderedVertexSet OrderedVertexSet::set_union(const OrderedVertexSet &s) const {

    std::vector<size_t> merged_vertices;

    size_t idx1 = 0, idx2 = 0;
    auto &s_vertices = s.get_vertices();

    // Implement list merge. Maintains sorted order assuming both lists are initially sorted.
    while ((idx1 < size()) || (idx2 < s_vertices.size())) {

        if (idx1 == size()) {
            merged_vertices.push_back(s_vertices[idx2]);
            idx2++;
        }
        else if (idx2 == s_vertices.size()) {
            merged_vertices.push_back(vertex_list[idx1]);
            idx1++;
        }
        else {
            if (vertex_list[idx1] == s_vertices[idx2]) {
                merged_vertices.push_back(vertex_list[idx1]);
                idx1++;
                idx2++;
            }
            else if (vertex_list[idx1] < s_vertices[idx2]) {
                merged_vertices.push_back(vertex_list[idx1]);
                idx1++;
            }
            else {
                merged_vertices.push_back(s_vertices[idx2]);
                idx2++;
            }
        }

    }

    return OrderedVertexSet(merged_vertices);

}

/*
 * Given another OrderedVertexSet s, return a new OrderedVertexSet r which is the intersection
 * of this set and s. Runtime is O(size of smaller set)
 */
std::vector<size_t> OrderedVertexSet::set_intersection_vec(const OrderedVertexSet &s) const {

    std::vector<size_t> intersection;

    if (size() < s.size()) {

        for (auto v : vertex_list) {
            if (s.has_vertex(v)) intersection.push_back(v);
        }

    }
    else {

        for (auto v : s) {
            if (has_vertex(v)) intersection.push_back(v);
        }

    }

    return intersection;

}


/*
 * Given another OrderedVertexSet s, return a new OrderedVertexSet r which is this set minus
 * all elements of this set which appear in s.
 */
std::vector<size_t> OrderedVertexSet::set_minus_vec(const OrderedVertexSet &s) const {

    std::vector<size_t> vertices;
    for (auto v : this->vertex_list) {
        if (!s.has_vertex(v)) vertices.push_back(v);
    }
    return vertices;
}


/*
 * Given an input vertex v, if v is contained in this, then return a new
 * OrderedVertexSet r which is this set minus all elements of this set
 * less than v. If v not in this, return r empty.
 */
std::vector<size_t> OrderedVertexSet::truncate_before_node_vec(size_t v) const {

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
std::vector<size_t> OrderedVertexSet::truncate_after_node_vec(size_t v) const {

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


/*
 * OrderedVertexSet::operator== returns true if the input s has the exact same contents as this.
 */
bool OrderedVertexSet::operator==(const OrderedVertexSet &s) const {

    if (size() != s.size()) return false;

    const std::vector<size_t> &s_vertex_list = s.get_vertices();

    return (this->vertex_list == s_vertex_list);

}

/**
 * Compare this OrderedVertexSet to another to determine if they are not equal.
 * Equality is determined by deferring to OrderedVertexSet::operator==.
 *
 * @param  s The OrderedVertexSet that `this` will be compared to.
 * @return   True if the sets are different, false if they are not.
 */
bool OrderedVertexSet::operator!=(const OrderedVertexSet &s) const {
    return !(*this == s);
}


/**
 * Compare this OrderedVertexSet to another to determine if this is
 * lexicographically least.
 * OrderedVertexSet::operator<.
 *
 * @param  s The OrderedVertexSet that `this` will be compared to.
 * @return   True if this < s, false otherwise.
 */
bool OrderedVertexSet::operator<(const OrderedVertexSet &s) const {
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
 * Check whether an input OrderedVertexSet s is contained in this.
 */
bool OrderedVertexSet::has_subset(const OrderedVertexSet &s) const {

    // if this is too small to contain s, return false
    if (size() < s.size()) return false;

    // Empty set always contained as a subset
    if (s.size() == 0) return true;

    // if range of s not contained in range of this, return false
    if (!contains_range(s.front(), s.back())) {
        return false;
    }

    // range of s inside range of this; must check each element
    for (auto j : s) {
        if (!has_vertex(j)) {return false;}
    }

    return true;

}

/*
 * Adds a vertex to the hashtable vertex_map, as well as the back of the vector
 * vertex_list. No sort is performed --- if the user adds a vertex that is
 * smaller than the current upper_bound, the result will be an incorrectly
 * OrderedVertexSet, and so the other functions will not work properly.
 */
void OrderedVertexSet::add_vertex_unsafe(size_t v) {

    if (has_vertex(v)) return;

    vertex_map[v] = true;
    vertex_list.push_back(v);

}


/*
 * Adds a vertex safely.
 */
void OrderedVertexSet::add_vertex_safe(size_t v) {

    if (has_vertex(v)) return;

    // first increase vector by one;
    vertex_list.push_back( v ); // will correct value later
    vertex_map[v] = true;

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
std::string OrderedVertexSet::to_string() const {
    std::stringstream s;

    if (vertex_list.size() == 0) return std::string("");

    for (auto v : vertex_list) {
        s << v << ",";
    }

    return s.str();
}


/**
 * Convert an OVS of vertices by relabeling vertices using input labels.
 *
 * Overloading function to run on a single OVS.
 */
OrderedVertexSet convert_node_labels_OVS(const OrderedVertexSet & list_to_convert,
                                         const OrderedVertexSet & label_converter){
    std::vector<size_t> temp_vector;
    for (auto v: list_to_convert) {
        temp_vector.push_back(label_converter[v]); // label conversion
    }
    std::sort(temp_vector.begin(), temp_vector.end());

    return OrderedVertexSet(temp_vector);

}


/**
 * The MIS are computed using labels of subgraph.
 * Must convert each MIS to use labelling of graph.
 *
 * Overloading function to run on a list of OVS.
 */
 std::vector<OrderedVertexSet> convert_node_labels_OVS(const std::vector<OrderedVertexSet> & list_of_sets,
                                                       const OrderedVertexSet & label_converter){

    std::vector<OrderedVertexSet> res;
    for (auto this_set: list_of_sets) {
        res.push_back(convert_node_labels_OVS(this_set, label_converter));
    }

    return res;
}


void convert_node_labels_OVS_inplace(std::vector<OrderedVertexSet> & list_of_sets,
                                     const OrderedVertexSet & label_converter);
