/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* Implements BFS for counting number of connected components.
* Given a graph, g, this code outputs a vector of vectors, one vector
* for each connected component of g. Each connected component vector
* contains the nodes of that CC in ascending order.
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include "SimpleCCs.h"
#include <queue>
#include <algorithm>  // for std::sort

std::vector<std::vector<size_t>> simpleccs(const Graph & g){

std::vector<std::vector<size_t>> res;
// Data structures for tracking node visitation
size_t num_vertices = g.get_num_vertices();
std::vector<bool> already_visited(num_vertices);
std::queue<size_t> vertex_queue;

// Iterate the BFS over all vertices
for (size_t idx = 0; idx < num_vertices; idx++) {

    if (already_visited[idx] == true) continue;

    // add vertex to queue and explore
    std::vector<size_t> connected_component;
    vertex_queue.push(idx);
    already_visited[idx] = true;
    while (vertex_queue.size() > 0) {

        size_t this_node = vertex_queue.front();
        connected_component.push_back(this_node);
        auto & this_neighborhood = g.get_neighbors_vector(this_node);
        vertex_queue.pop();

        for (size_t neighbor : this_neighborhood) {
            if (already_visited[neighbor] == true) continue;
            vertex_queue.push(neighbor);
            already_visited[neighbor] = true;
        }

    }

    // Sort the connected component and add to output
    std::sort(connected_component.begin(), connected_component.end());
    res.push_back(connected_component);

}

return res;

}
