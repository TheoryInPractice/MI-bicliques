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


int test_graph_biclique_string(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Create BicliqueLite object
    size_t dummy;
    g.get_internal_vertex_label("dingo", dummy);
    std::vector<size_t> left_labels = {dummy};

    std::vector<size_t> right_labels;
    std::vector<std::string> string_labels = {"elephant","ferret","guppy"};  // correct order hard-coded
    for (auto s : string_labels) {
        g.get_internal_vertex_label(s, dummy);
        right_labels.push_back(dummy);
    }
    std::sort(right_labels.begin(), right_labels.end());

    std::vector<size_t> left = left_labels;
    std::vector<size_t> right = right_labels;

    BicliqueLite correct_biclique(left, right);

    // Get string from Graph and check accuracy
    std::string computed_string = g.biclique_string(correct_biclique);
    std::string correct_biclique_string = "ferret,elephant,dingo,guppy,";  // correct order hard-coded

    if ( computed_string != correct_biclique_string ) {
        std::cout << "Computed biclique string incorrect: " << computed_string << std::endl;
        error = true;
    }

    return error;

}
