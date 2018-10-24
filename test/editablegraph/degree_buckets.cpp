/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/graph/Graph.h"
#include"../../src/graph/EditableGraph.h"

/**
 * Test the constructor in the class EditableGraph. This constructor takes as
 * input a Graph object, and checks that the adjacency structure is correctly
 * read in as an EditableGraph.
 */
int test_editablegraph_degree_buckets(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // read in Graph object
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    EditableGraph eg(g);

    // Check degree buckets are correct
    for (size_t degree_eg = 0; degree_eg < 3; degree_eg++ ) {

        auto bucket = eg.degree_buckets[degree_eg];

        for (const auto &iter : bucket) {
            size_t vertex = iter.first;
            size_t degree_actual = g.get_neighbors(vertex).size();
            if (degree_eg != degree_actual) {
                std::cout << "EditableGraph: vertices in degree bucket " << degree_eg << " should have degree " << degree_eg << ", not " << degree_actual << std::endl;
                error = true;
            }
        }
    }

    // Now check bucket with nodes of degree >= 3
    for (const auto &iter : eg.degree_buckets[3]) {
        size_t vertex = iter.first;
        size_t degree_actual = g.get_neighbors(vertex).size();
        if (degree_actual < 3) {
            std::cout << "EditableGraph: vertices in degree bucket 3 should have degree >= 3, not " << degree_actual << std::endl;
            error = true;
        }
    }


    // Now hard code degrees after each of the deletions
    std::vector<std::vector<size_t>> altered_degrees = {
            (std::vector<size_t>) {2, 2, 2},
            (std::vector<size_t>) {3, 3, 3},
            (std::vector<size_t>) {3, 2, 2},
            (std::vector<size_t>) {3, 2, 1},
            (std::vector<size_t>) {4},
            (std::vector<size_t>) {3, 2},
            (std::vector<size_t>) {2, 1, 0}
    };

    for (size_t counter = 0; counter < 3; counter++) {

        switch (counter) {
            case 0:
                break;
            case 1:
                eg.delete_node(4);
                break;
            case 2:
                eg.delete_node(5);
        }

        // Skip deleted vertices
        for (const auto &iter : eg) {
            size_t vertex = iter.first;
            size_t current_degree = eg.get_neighbors(vertex).size();

            // if actual degree different from hard-coded degree, error
            if (current_degree != altered_degrees[vertex][counter]){
                std::cout << "EditableGraph: vertex " << vertex << " has wrong degree (" << current_degree << ") after deletion " << counter << std::endl;
                error = true;
            }

            // check each vertex is in correct degree bucket
            std::unordered_map<size_t,bool> current_bucket = {};
            if (current_degree>=3) {current_bucket = eg.degree_buckets[3];}
            else {current_bucket = eg.degree_buckets[current_degree];}

            if (current_bucket.find(vertex) == current_bucket.end()) {
                std::cout << "EditableGraph: vertex " << vertex << " should be in bucket for degree " << current_degree << std::endl;
                error = true;
            }

        }
    }

    return error;

}
