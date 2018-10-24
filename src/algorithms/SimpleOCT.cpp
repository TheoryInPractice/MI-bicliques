/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include <queue>

#include "SimpleOCT.h"


void simpleoct(const Graph & g,
                 OrderedVertexSet & oct_set,
                 OrderedVertexSet & left_partition,
                 OrderedVertexSet & right_partition
             ){

// Data structures for tracking node color and visitation
size_t num_vertices = g.get_num_vertices();
std::vector<size_t> color_vector, already_visited, neighbor_colors;
color_vector.resize(num_vertices);
already_visited.resize(num_vertices);
neighbor_colors.resize(num_vertices);

std::queue<size_t> vertex_queue;

// Check if OCT set prescribed. If yes, color OCT nodes as 3,
if (oct_set.size() > 0) {
    for (auto v: oct_set) {
        color_vector[v] = 3;
    }
}

size_t first_vertex = 0;
size_t number_of_vertices_visited = 0;

// Make sure first vertex is not isolated
for (size_t idx = 0; idx < g.get_num_vertices(); idx++) {
    if (g.get_neighbors(idx).size() == 0) {
        already_visited[idx] = 1;
        number_of_vertices_visited++;
        color_vector[idx] = 1;
        continue;
    }
    first_vertex = idx;
    break;
}

// If OCT set prescribed, find first vertex not in OCT.
if (oct_set.size() > 0) {
    for (size_t idx = 0; idx < g.get_num_vertices(); idx++) {
        if (oct_set.has_vertex(idx)) continue;
        else {
            first_vertex = idx;
            break;
        }
    }
}

// Color the first node 1 (red) and add neighbors to queue
color_vector[first_vertex] = 1;
for (auto v : g.get_neighbors(first_vertex)) {
    neighbor_colors[v] = 1;
    vertex_queue.push(v);
}
already_visited[first_vertex] = 1;
number_of_vertices_visited++;


// loop until all connected components have been colored
while (number_of_vertices_visited < g.get_num_vertices()) {

    while (vertex_queue.size() > 0) {

        // get next node that hasn't yet been visited.
        size_t current_vertex = vertex_queue.front();

        already_visited[current_vertex] = 1;
        number_of_vertices_visited++;

        // If current_vertex already colored 3, it's in OCT -> do nothing
        size_t current_color = color_vector[current_vertex];
        if (current_color != 3) {
            // Set to color 1 unless forced to use 2;
            // use 3 if it already has neighbors with colors 1 and 2
            current_color = 1;
            auto neighborhood_colors = neighbor_colors[current_vertex];
            if (neighborhood_colors == 1) current_color = 2;
            else if (neighborhood_colors == 3) current_color = 3;

            color_vector[current_vertex] = current_color;
        }


        // For each unvisited v in N(current_vertex) update neighbor_colors[v]
        const OrderedVertexSet & neighbors = g.get_neighbors(current_vertex);
        for (const auto v : neighbors) {
            if (already_visited[v] == 1) continue;

            size_t this_neighborhood = neighbor_colors[v];
            if ((current_color == 3) || (this_neighborhood == 3)) {
                // Do nothing:
                // If current_color == 3, current_vertex is in OCT, so
                // we're not adding a color to neighbor_colors[v].
                // If neighbor_colors[v] is already 3, v has all colors already.
            }
            else if ((this_neighborhood == 0) || (this_neighborhood == current_color)) {
                // If v has seen no colors, or only seen current_color,
                // then neighbor_colors[v] consists of {current_color}.
                neighbor_colors[v] = current_color;
            }
            else {
                // Else, v has all colors in its neighborhood, so set 3.
                neighbor_colors[v] = 3;
            }

            vertex_queue.push(v);
        }

        vertex_queue.pop();
    }

    // Check if there are any unvisited vertices *not* in the OCT set
    for (size_t dummy = first_vertex+1; dummy < g.get_num_vertices(); dummy++) {
        if (already_visited[dummy] == 0 && !oct_set.has_vertex(dummy)) {
            first_vertex = dummy;

            // Color the first node of this connected component 1 (red)
            // then add neighbors to queue
            color_vector[first_vertex] = 1;
            for (auto v : g.get_neighbors(first_vertex)) {
                neighbor_colors[v] = 1;
                vertex_queue.push(v);
            }
            already_visited[first_vertex] = 1;
            number_of_vertices_visited++;

            break;
        }
    }
}

// Every vertex has been colored --- now sort into OCT decomposition
for (size_t idx = 0; idx < num_vertices; idx++) {

    size_t color = color_vector[idx];

    // if color still 0, independent node
    if (color == 0) left_partition.add_vertex_unsafe(idx);
    else if (color == 1) left_partition.add_vertex_unsafe(idx);
    else if (color == 2) right_partition.add_vertex_unsafe(idx);
    else oct_set.add_vertex_unsafe(idx); // (color == 3)
}

return;

}
