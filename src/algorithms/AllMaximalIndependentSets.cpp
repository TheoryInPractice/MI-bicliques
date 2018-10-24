/**
* This implements the algorithm of Eppstein for enumerating all maximal
* independent sets in a general graph, presented here:
*
*   Eppstein, David. "Small maximal independent sets and faster exact graph
*   coloring." Workshop on Algorithms and Data Structures. Springer, Berlin,
*   Heidelberg, 2001.
*
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include "../graph/Graph.h"
#include "../graph/OrderedVertexSet.h"
#include "AllMaximalIndependentSets.h"


/**
 * Check whether input node set "independent_set" can have any vertex in input
 * graph_ptr added to it (i.e. whether it is maximal as an independent set).
 */
bool check_is_maximal(std::shared_ptr<Graph> graph_ptr, OrderedVertexSet& independent_set){

    // for each vertex outside the set, see if it can be added
    for (auto vertex = 0; vertex < graph_ptr->get_num_vertices(); vertex++){

        // Skip vertices in the independent_set
        if (independent_set.has_vertex(vertex)) continue;

        // If we find a vertex that's independent from input, output not maximal
        if (graph_ptr->is_completely_independent_from(vertex,independent_set)) return false;
    }

    return true;
}


/**
 * Recursively branches on nodes in ed_graph that can be added to the
 * set independent_set. This is the main subroutine of the MIS enumeration
 * algorithm of Dias et al.
 */
void recursive_eppstein(std::shared_ptr<Graph> graph_ptr,
                        std::vector<std::vector<size_t>>& list_of_MIS,
                        EditableGraph& ed_graph,
                        std::unordered_map<size_t,bool>& independent_set,
                        size_t size_limit
                       ) {

    // if S = 0 or size_limit = 0, check to add to the list of MISs
    if ( (ed_graph.size()==0) || (size_limit == 0) ){

        // faster to check maximality using an OVS
        OrderedVertexSet IS(independent_set);

        // if it's maximal, add to the list
        if (check_is_maximal(graph_ptr,IS)) {
            list_of_MIS.push_back(IS.get_vertices());
        }

        return;

    }

    // DEGREE 0: subgraph contains vertices of degree exactly 0
    else if (ed_graph.degree_buckets[0].size() > 0) {

        // put all degree zero vertices into independent set
        auto temp_bucket = ed_graph.degree_buckets[0];
        for (auto const & iter : temp_bucket) {
            size_t vertex = iter.first;

            ed_graph.delete_node(vertex); // delete vertex from subgraph

            independent_set[vertex] = true; // add v to independent set

            size_limit--;  // decrement for each vertex deleted
        }

        recursive_eppstein(graph_ptr,
                           list_of_MIS,
                           ed_graph,
                           independent_set,
                           size_limit
                          );

        return;
    }

    // DEGREE 1: subset_of_graph contains a vertex of degree exactly 1
    else if (ed_graph.degree_buckets[1].size() > 0) {

        // use the first vertex (v) we can find with degree 1
        size_t vertex = ed_graph.degree_buckets[1].begin()->first;

        // get only neighbor (u) of vertex (v)
        size_t neighbor = ed_graph.get_neighbors(vertex).begin()->first;

        // FIRST recursive call: (S-N(v), I+v, k-1)
        // create subgraph minus the deleted vertex
        EditableGraph ed_graph_v = ed_graph;
        ed_graph_v.delete_neighbors(vertex);

        // copy independent set
        auto independent_set_copy = independent_set;
        independent_set_copy[vertex] = true;

        // make recursive call
        recursive_eppstein(graph_ptr,
                           list_of_MIS,
                           ed_graph_v,
                           independent_set_copy,
                           size_limit - 1
                          );

        // SECOND recursive call: (S-N(u), I+u, k-1)
        // Delete N(u) from graph
        ed_graph.delete_neighbors(neighbor);

        // add neighbor to independent set
        independent_set[neighbor] = true;

        recursive_eppstein(graph_ptr,
                           list_of_MIS,
                           ed_graph,
                           independent_set,
                           size_limit - 1
                          );

        return;
    }

    // DEGREE 3+: if subset_of_graph contains a vertex with induced degree >= 3
    else if (ed_graph.degree_buckets[3].size() > 0) {

        // use the first vertex we can find with degree 3+
        size_t vertex = ed_graph.degree_buckets[3].begin()->first;

        // FIRST recursive call: (S-v, I, k)
        // create subgraph minus the deleted vertex
        EditableGraph ed_graph_v = ed_graph;
        ed_graph_v.delete_node(vertex);

        // copy independent set
        auto independent_set_copy = independent_set;

        // make recursive call
        recursive_eppstein(graph_ptr,
                           list_of_MIS,
                           ed_graph_v,
                           independent_set_copy,
                           size_limit
                          );

        // SECOND recursive call: (S-N(v), I+v, k-1)
        // Delete N(v) from graph
        ed_graph.delete_neighbors(vertex);

        // add vertex to independent set
        independent_set[vertex] = true;

        recursive_eppstein(graph_ptr,
                           list_of_MIS,
                           ed_graph,
                           independent_set,
                           size_limit - 1
                          );

        return;
    }


    // ALL VERTICES DEGREE 2
    // FIRST recursive call: (S-N(v), I+v, k-1)
    // SECOND recursive call: (S-N(u), I+u, k-1)
    // THIRD recursive call: (S-N(w)-u, I+w, k-1)
    else {

        // use the first vertex (v) we can find with degree 2
        size_t vertex = ed_graph.degree_buckets[2].begin()->first;

        std::vector<size_t> neighbors = {};
        for (auto const & iter : ed_graph.get_neighbors(vertex)) neighbors.push_back(iter.first);
        size_t neighb_u = neighbors[0];
        size_t neighb_w = neighbors[1];

        // FIRST remove N(v)
        EditableGraph ed_graph_v = ed_graph;
        ed_graph_v.delete_neighbors(vertex);

        // copy independent set
        auto independent_set_copy = independent_set;
        independent_set_copy[vertex] = true;

        // make recursive call
        recursive_eppstein(graph_ptr,
                           list_of_MIS,
                           ed_graph_v,
                           independent_set_copy,
                           size_limit - 1
                          );

        // SECOND remove N(u)
        EditableGraph ed_graph_u = ed_graph;
        ed_graph_u.delete_neighbors(neighb_u);

        // copy independent set
        auto independent_set_copy_u = independent_set;
        independent_set_copy_u[neighb_u] = true;

        // make recursive call
        recursive_eppstein(graph_ptr,
                           list_of_MIS,
                           ed_graph_u,
                           independent_set_copy_u,
                           size_limit - 1
                          );

        // SECOND remove N(w) and u
        ed_graph.delete_neighbors(neighb_w);
        ed_graph.delete_node(neighb_u);

        // update independent set
        independent_set[neighb_w] = true;

        // make recursive call
        recursive_eppstein(graph_ptr,
                           list_of_MIS,
                           ed_graph,
                           independent_set,
                           size_limit - 1
                          );

        return;
    }

}


/**
 * Wrapper for the maximal-independent-set enumeration algorithm.
 * Given input graph g, output a vector containing all maximal independent sets
 * in g. Each such independent set is stored as a vector containing node
 * indices in ascending order.
 */
std::vector<std::vector<size_t>> get_all_mis(Graph & g) {

    // Initializations
    // Get shared_ptr for graph
    auto graph_ptr = std::make_shared<Graph>(g);

    // Get editable graph
    EditableGraph ed_graph(g);

    // Output will be vector of MISs
    std::vector<std::vector<size_t>> list_of_MIS;

    // initialize full list of graph's vertices
    size_t num_vertices = g.get_num_vertices();

    // initialize empty set
    std::unordered_map<size_t,bool> potential_MIS;

    recursive_eppstein(graph_ptr,
                       list_of_MIS,
                       ed_graph,
                       potential_MIS,
                       num_vertices
                      );


    return list_of_MIS;
}
