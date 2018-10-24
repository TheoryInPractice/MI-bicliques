/**
 * OctGraph is a container class derived from the class Graph which offers
 * functions for accessing partitions and neighborhood information in a graph
 * with an Odd-Cycle-Transversal (oct) decomposition.
 * See OctGraph.cpp for full documentation.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

#ifndef BICLIQUES_OCTGRAPH_H
#define BICLIQUES_OCTGRAPH_H


#include "Graph.h"

class OctGraph : public Graph {

private:
    std::vector<OrderedVertexSet> oct_neighbors,
                                  left_neighbors,
                                  right_neighbors;
    std::vector<OrderedVertexSet *> node_to_partition;
    OrderedVertexSet oct, left, right;

public:
    OctGraph(const Graph &input_graph,
             OrderedVertexSet oct,
             OrderedVertexSet left,
             OrderedVertexSet right);
    OctGraph(size_t num_vertices);

    const OrderedVertexSet *get_node_partition(size_t) const;
    const OrderedVertexSet& get_oct() const;
    const OrderedVertexSet& get_left() const;
    const OrderedVertexSet& get_right() const;
    const OrderedVertexSet& get_oct_neighbors(size_t v) const;
    const OrderedVertexSet& get_left_neighbors(size_t v) const;
    const OrderedVertexSet& get_right_neighbors(size_t v) const;

};


#endif //BICLIQUES_OCTGRAPH_H
