/**
 * OrderedVertexSet is a container class for efficient set operations.
 * See OrderedVertexSet.cpp for full documentation.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

#ifndef BICLIQUES_VERTEXSET_H
#define BICLIQUES_VERTEXSET_H


#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>


inline std::string vector_to_string(const std::vector<size_t> & vec) {
    std::stringstream s;

    if (vec.size() == 0) return std::string("");

    for (auto v : vec) {
        s << v << ",";
    }

    return s.str();
}


class OrderedVertexSet {

private:
    std::unordered_map<size_t, bool> vertex_map;
    std::vector<size_t> vertex_list;

public:
    OrderedVertexSet();
    OrderedVertexSet(std::vector<size_t> vertex_list);
    OrderedVertexSet(std::unordered_map<size_t, bool>);

    OrderedVertexSet set_union(const OrderedVertexSet &s) const;

    std::vector<size_t> set_intersection_vec(const OrderedVertexSet &s) const;
    inline OrderedVertexSet set_intersection(const OrderedVertexSet &s) const {
        return OrderedVertexSet(set_intersection_vec(s));
    }

    std::vector<size_t> set_minus_vec(const OrderedVertexSet &s) const;
    inline OrderedVertexSet set_minus(const OrderedVertexSet &s) const {
        return OrderedVertexSet(set_minus_vec(s));
    }

    std::vector<size_t> truncate_before_node_vec(size_t v) const;
    inline OrderedVertexSet truncate_before_node(size_t & v) const {
        return OrderedVertexSet(truncate_before_node_vec(v));
    }

    std::vector<size_t> truncate_after_node_vec(size_t v) const;
    inline OrderedVertexSet truncate_after_node(size_t & v) const {
        return OrderedVertexSet(truncate_after_node_vec(v));
    }


    bool operator==(const OrderedVertexSet &other) const;
    bool operator!=(const OrderedVertexSet &other) const;
    bool operator<(const OrderedVertexSet &other) const;

    inline void clear(){
        vertex_map.clear();
        vertex_list.clear();
    }

    // Return reference to the vector vertex_list.
    inline const std::vector<size_t>& get_vertices() const {
        return vertex_list;
    };

    void add_vertex_unsafe(size_t);
    void add_vertex_safe(size_t v);

    /*
     * Given the largest and smallest items from another set, check whether they are
     * entirely within the interval defined by the largest and smallest items
     * from this set.
     */
    inline bool contains_range(const size_t & lower_bound, const size_t & upper_bound) const {

        if ((lower_bound < vertex_list.front()) || (upper_bound > vertex_list.back())){
            return false;
        }

        return true;
    }

    /*
     * Given the largest and smallest items from another set, check whether
     * they are entirely outside the interval defined by the largest and smallest
     * items from this set. Note that this function does *not* simply return the
     * negation of the function contains_range().
     */
    inline bool disjoint_from_range(const size_t & lower_bound, const size_t & upper_bound) const {

        if ((upper_bound < vertex_list.front()) || (lower_bound > vertex_list.back())){
            return true;
        }

        return false;
    }

    bool has_subset(const OrderedVertexSet &s) const;
    inline bool has_vertex(const size_t & v) const {
        if (vertex_map.find(v) != vertex_map.end()) {
            return true;
        }
        return false;
    }

    inline size_t size() const { return vertex_list.size(); }
    inline size_t operator[] (const size_t & idx) const { return vertex_list[idx]; }

    inline size_t front() const { return vertex_list.front(); }
    inline size_t back() const { return vertex_list.back(); }

    // define pieces for iterator
    inline std::vector<size_t>::iterator begin() {return vertex_list.begin();}
    inline std::vector<size_t>::iterator end() {return vertex_list.end();}
    inline std::vector<size_t>::const_iterator begin() const {return vertex_list.begin();}
    inline std::vector<size_t>::const_iterator end() const {return vertex_list.end();}

    std::string to_string() const;

};


OrderedVertexSet convert_node_labels_OVS(const OrderedVertexSet & particular_set,
                                         const OrderedVertexSet & label_converter);

std::vector<OrderedVertexSet> convert_node_labels_OVS(const std::vector<OrderedVertexSet> & list_of_sets,
                                                      const OrderedVertexSet & label_converter);

/**
 * Convert an OVS of vertices by relabeling vertices using input labels.
 *
 * Overloading function to run on a single OVS.
 */
 template <typename T>
std::vector<size_t> convert_node_labels_vector(const std::vector<size_t> & list_to_convert,
                                            const T & label_converter){
    std::vector<size_t> temp_vector;
    for (auto v: list_to_convert) {
        temp_vector.push_back(label_converter[v]); // label conversion
    }
    std::sort(temp_vector.begin(), temp_vector.end());

    return temp_vector;

}


/**
 * The MIS are computed using labels of subgraph.
 * Must convert each MIS to use labelling of graph.
 *
 * Overloading function to run on a list of OVS.
 */
 template <typename T>
 std::vector<std::vector<size_t>> convert_node_labels_vector(const std::vector<std::vector<size_t>> & list_of_sets,
                                                             const T & label_converter){

    std::vector<std::vector<size_t>> res;
    for (auto this_set: list_of_sets) {
        res.push_back(convert_node_labels_vector(this_set, label_converter));
    }

    return res;
}


/**
 * Convert an OVS of vertices by relabeling vertices using input labels.
 *
 * Overloading function to run on a single OVS.
 */
 template <typename T>
void convert_node_labels_vector_inplace(std::vector<size_t> & list_to_convert,
                                        const T & label_converter){
    for (size_t idx = 0; idx < list_to_convert.size(); idx++) {
        list_to_convert[idx] = label_converter[list_to_convert[idx]];
    }
    std::sort(list_to_convert.begin(), list_to_convert.end());

}


/**
 * The MIS are computed using labels of subgraph.
 * Must convert each MIS to use labelling of graph.
 *
 * Overloading function to run on a list of OVS.
 */
 template <typename T>
 void convert_node_labels_vector_inplace(std::vector<std::vector<size_t>> & list_of_sets,
                                         const T & label_converter){

    for (auto & this_set: list_of_sets) {
        convert_node_labels_vector_inplace(this_set, label_converter);
    }
}


#endif //BICLIQUES_VERTEXSET_H
