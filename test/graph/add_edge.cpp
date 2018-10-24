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

int test_graph_add_edge(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph_small.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    if (g.add_edge_unsafe("aardvark", "capybara")) {
        std::cout << "Edge (aardvark, capybara) should already exist." << std::endl;
        error = true;
    }
    if (!g.add_edge_unsafe("ferret", "capybara")) {
        std::cout << "Edge (ferret, capybara) should not already exist." << std::endl;
        error = true;
    }
    if (g.add_edge_unsafe("capybara", "ferret")) {
        std::cout << "Edge (ferret, capybara) was just added, should exist." << std::endl;
        error = true;
    }

    return error;

}
