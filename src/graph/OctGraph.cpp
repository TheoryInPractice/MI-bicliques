/**
 * OctGraph is a container class derived from the class Graph.
 * In addition to the capabilities of Graph, OctGraph encodes the partitions
 * of a graph with an Odd-Cycle-Transversal (oct) set, which separates a graph
 * into three partitions: partitions left and right, which form a bipartite
 * graph together, and a third partition, oct.
 * This class gives convenient functions for accessing these different
 * partitions, as well as vertex neighborhoods induced by these partitions.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

#include "OctGraph.h"


OctGraph::OctGraph(const Graph &input_graph,
                   OrderedVertexSet oct,
                   OrderedVertexSet left,
                   OrderedVertexSet right)
                   :
                   Graph(input_graph),
                   oct(oct),
                   left(left),
                   right(right)
{
    oct_neighbors.resize(num_vertices);
    left_neighbors.resize(num_vertices);
    right_neighbors.resize(num_vertices);
    node_to_partition.resize(num_vertices);

    for (auto v : oct) node_to_partition[v] = &(this->oct);
    for (auto v : left) node_to_partition[v] = &(this->left);
    for (auto v : right) node_to_partition[v] = &(this->right);

    for (auto v : this->oct) {
        const OrderedVertexSet neighb = adjacency_ordered_vertex_sets[v];
        oct_neighbors[v] = neighb.set_intersection(this->oct);
        left_neighbors[v] = neighb.set_intersection(this->left);
        right_neighbors[v] = neighb.set_intersection(this->right);
    }

}

OctGraph::OctGraph(size_t num_vertices) : Graph(num_vertices)
{

    oct_neighbors.resize(num_vertices);
    left_neighbors.resize(num_vertices);
    right_neighbors.resize(num_vertices);

    node_to_partition.resize(num_vertices);

}


const OrderedVertexSet& OctGraph::get_oct() const {

    return oct;

}


const OrderedVertexSet& OctGraph::get_left() const {

    return left;

}


const OrderedVertexSet& OctGraph::get_right() const {

    return right;

}


const OrderedVertexSet& OctGraph::get_oct_neighbors(size_t v) const {

    return oct_neighbors[v];

}


const OrderedVertexSet& OctGraph::get_left_neighbors(size_t v) const {

    return left_neighbors[v];

}


const OrderedVertexSet& OctGraph::get_right_neighbors(size_t v) const {

    return right_neighbors[v];

}


const OrderedVertexSet *OctGraph::get_node_partition(size_t v) const {

    return node_to_partition[v];

}
