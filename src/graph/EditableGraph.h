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

#ifndef BICLIQUES_EDITABLEGRAPH_H
#define BICLIQUES_EDITABLEGRAPH_H


#include <vector>
#include <unordered_map>

#include "Graph.h"


class EditableGraph {

private:
    std::vector<std::unordered_map<size_t,bool>> adj_dict;
    std::unordered_map<size_t,bool> vertex_list;

public:
    EditableGraph(Graph graph);

    inline size_t size() const { return vertex_list.size(); }

    std::vector<std::unordered_map<size_t,bool>> degree_buckets;

    void delete_node(size_t vertex);
    void delete_neighbors(size_t vertex);

    inline bool has_vertex(size_t v) const {
        return (vertex_list.find(v) != vertex_list.end());
    }

    inline const std::unordered_map<size_t,bool> & get_neighbors(size_t vertex) const {
        return adj_dict[vertex];
    }

    /*
     * Necessary to be able to iterate over the vertex_list of EditableGraph
     */
    inline std::unordered_map<size_t,bool>::iterator begin() { return vertex_list.begin(); }
    inline std::unordered_map<size_t,bool>::iterator end() { return vertex_list.end(); }


};


#endif //BICLIQUES_EDITABLEGRAPH_H
