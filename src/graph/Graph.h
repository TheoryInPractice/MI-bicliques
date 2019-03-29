/**
 * Graph is a container class for encoding a graph with functions for
 * efficiently determining connectivity of subsets of vertices.
 * See Graph.cpp for full documentation.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

#ifndef BICLIQUES_GRAPH_H
#define BICLIQUES_GRAPH_H

#include <cstddef>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>


#include "OrderedVertexSet.h"
#include "BicliqueLite.h"




class Graph {

protected:

    size_t num_vertices, num_edges;
    std::unordered_map<size_t, std::string> node_labels;
    std::unordered_map<std::string, size_t> reverse_node_labels;

    std::vector<bool> adjacency_matrix;
    std::vector<std::vector<size_t>> adjacency_list;
    std::vector<OrderedVertexSet> adjacency_ordered_vertex_sets;

    void read_edgelist(std::string filename);
    void read_adjlist(std::string filename);

    bool add_edge(size_t u, size_t v);

public:

    enum FILE_FORMAT {edgelist, adjlist};

    Graph(size_t num_vertices);
    Graph(std::string filename, FILE_FORMAT format);
    Graph(const std::vector<size_t> &s,
          const std::vector<std::vector<size_t>> &supergraph_adjacency_list,
          const std::unordered_map<size_t, std::string> &supergraph_labels);

    inline const OrderedVertexSet& get_neighbors(const size_t v) const {
        return this->adjacency_ordered_vertex_sets[v];
    }
    inline const std::vector<size_t>& get_neighbors_vector(const size_t v) const {
        return this->adjacency_list[v];
    }


    Graph subgraph(const std::vector<size_t> &s) const;
     // Wrapper subgraph(T) for T = OVS and OrderedVector
    template <typename T>
    Graph subgraph(const T &s) const {
        return this->subgraph(s.get_vertices());
    }

    bool add_edge_unsafe(std::string u, std::string v);
    inline bool has_edge(size_t u, size_t v) const {
        return adjacency_matrix[num_vertices * u + v];
    }
    bool has_edge(const std::string & u, const std::string & v);

    // Template allows function to work on OVS and OrderedVector
    template<class T>
    inline bool is_completely_connect_to(const size_t v,
                                         const T &s) const {
        if (s.has_vertex(v)) return false;
        return is_completely_connect_to( v, s.get_vertices() );
    }
    // WARNING: assumes input vector is sorted.
    bool is_completely_connect_to(const size_t v,
                                  const std::vector<size_t> &s) const;

    // Template allows function to work on OVS and OrderedVector
    template<class T>
    inline bool is_completely_independent_from(const size_t v,
                                               const T &s) const {
        if (s.has_vertex(v)) return false;
        return is_completely_independent_from( v, s.get_vertices() );
    }
    // WARNING: assumes input vector is sorted.
    bool is_completely_independent_from(const size_t v,
                                        const std::vector<size_t> &s) const;

    size_t get_num_vertices() const;
    size_t get_num_edges() const;

    bool get_internal_vertex_label(const std::string & external_vertex_label, size_t& internal_vertex_label);
    std::string get_external_vertex_label(size_t internal_vertex_label);

    // For recording bicliques for comparing to ground truth
    std::string biclique_string(const BicliqueLite &b) const;
    bool can_be_added_to_biclique(const size_t v, const BicliqueLite &b) const;
    bool is_biclique(const BicliqueLite & this_biclique) const;


    /**
     *  Computes the intersection of the neighborhoods of the vertices in input_set,
     * i.e., the focus of input_set.  If exclusion_set is non-empty, then its
     * elements are excluded from the the returned set. If the variable
     * stop_at_first_node is true, then the function terminates as soon as
     * it is determined that the return set would be non-empty.
     *
     * WARNING: assumes input_set is sorted in ascending order.
     */
    template<class T>
    std::vector<size_t> get_neighborhood_intersection(const T & input_set,
                                   OrderedVertexSet & exclusion_set,
                                   bool stop_at_first_node = false) const {

       if (input_set.size() == 0) return std::vector<size_t>();

       size_t vertex_with_smallest_neighb = input_set.front();
       size_t smallest_neigb_size = (this->adjacency_list[vertex_with_smallest_neighb]).size();
       size_t lower_bound = 0;
       size_t upper_bound = this->num_vertices;

       // Find smallest neighborhood among those input,
       // as well as upper/lower bounds for neighborhoods in question.
       for (auto v: input_set) {

           size_t this_neighb_size = this->adjacency_list[v].size();

           if (lower_bound < this->adjacency_list[v].front())
               lower_bound = this->adjacency_list[v].front();
           if (upper_bound > this->adjacency_list[v].back())
               upper_bound = this->adjacency_list[v].back();

           if (lower_bound > upper_bound) return std::vector<size_t>();

           if ( this_neighb_size < smallest_neigb_size ) {
               vertex_with_smallest_neighb = v;
               smallest_neigb_size = this_neighb_size;
           }
       }

       // Smallest neighborhood found, now prune using upper/lower bounds.
       std::vector<size_t> temp_set = {};
       for (auto v: this->adjacency_ordered_vertex_sets[vertex_with_smallest_neighb]) {
           if (v < lower_bound) continue;
           if (v > upper_bound) break;

           temp_set.push_back(v);
       }

       // If exclusion set non-empty, remove it from pruned-set
       std::vector<size_t> pruned_set = {};
       if (exclusion_set.size() > 0) {
           for (auto v: temp_set) {
               if (!exclusion_set.has_vertex(v)) pruned_set.push_back(v);
           }
       }
       else {
           pruned_set = temp_set;
       }

       // Now for each v in pruned_set, check if its completely connected to input set
       std::vector<size_t> res = {};
       for (size_t v: pruned_set) {
           if (is_completely_connect_to(v, input_set)) {
               res.push_back(v);
               if (stop_at_first_node) {
                   return res;
               }
           }
       }

       return res;
    }
    /**
    * Overloading function to enable empty parameters
    */
    template<class T>
    inline std::vector<size_t> get_neighborhood_intersection(const T & input_set,
                                   bool stop_at_first_node = false) const {
        OrderedVertexSet dummy;
        return get_neighborhood_intersection(input_set, dummy, stop_at_first_node );
    }


    void print_graph() const;
    void print_graph_with_labels() const;

};

#endif //BICLIQUES_GRAPH_H
