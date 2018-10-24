/**
 * EditableGraph is a container class that enables efficient vertex
 * and neighborhood deletions, as well as access to low-degree nodes.
 * An OrderedVertexSet can be created from a Graph.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

#include "EditableGraph.h"


/*
 * Construct a new editable-graph instance by reading from a Graph object.
 *
 * @param graph   Graph object.
 */
EditableGraph::EditableGraph(Graph graph)
{

    size_t num_vertices = graph.get_num_vertices();
    this->adj_dict.resize(num_vertices);

    // populate vector of neighborhood dictionaries
    for (size_t vertex = 0; vertex < num_vertices; vertex++) {

        this->vertex_list[vertex] = true;

        std::unordered_map<size_t,bool> map_neighborhod;
        const OrderedVertexSet & neighborhood = graph.get_neighbors(vertex);

        for (auto neighbor : neighborhood) map_neighborhod[neighbor] = true;

        this->adj_dict[vertex] = map_neighborhod;

    }

    // Now populate degree dictionaries
    for (size_t idx = 0; idx < 4; idx++) {
        this->degree_buckets.push_back( (std::unordered_map<size_t,bool>) {});
    }

    for (auto vertex = 0; vertex < num_vertices; vertex++) {

        size_t degree = this->adj_dict[vertex].size();

        if (degree >= 3) this->degree_buckets[3][vertex] = true;
        else this->degree_buckets[degree][vertex] = true;

    }

}


/*
 * Delete an input node from the graph by removing it from vertex_list,
 * removing it from the neighborhoods of each of its neighbors, deleting
 * it from degree buckets and updating each of its neighbors in the
 * degree buckets, and deleting it from all data structures.
 *
 * WARNING: any time a node is removed from the graph, erase() must be called
 * or else has_vertex() will not function safely
 *
 * @param vertex      integer label of vertex to delete.
 */
void EditableGraph::delete_node(size_t vertex) {

    // remove vertex from degree_bucket entirely
    const auto neighborhood = this->adj_dict[vertex];
    size_t degree = neighborhood.size();
    if (degree>=3) degree_buckets[3].erase(vertex);
    else {
        degree_buckets[degree].erase(vertex);
    }

    // Delete vertex in each of its neighbors' neighborhoods
    for (const auto &iter : neighborhood) {

        size_t neighbor = iter.first;

        adj_dict[neighbor].erase(vertex);
        size_t new_degree = adj_dict[neighbor].size();

        // update degree information accordingly
        if (new_degree>=3) continue;
        else {
            degree_buckets[new_degree][neighbor] = true;
            degree_buckets[new_degree+1].erase(neighbor);
        }

    }

    // delete vertex's neighborhood itself
    adj_dict[vertex].clear();
    vertex_list.erase(vertex);

}


/*
 * Delete's a node's neighborhood by calling delete_node on each
 * of the neighbors of the input vertex.
 *
 * @param vertex    integer label of vertex whose neighborhood we delete.
 */
void EditableGraph::delete_neighbors(size_t vertex) {

    auto neighborhood = adj_dict[vertex];
    // Delete each neighbor
    for (const auto &iter : neighborhood) this->delete_node(iter.first);

}
