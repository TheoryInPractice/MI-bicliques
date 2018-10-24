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
#include"../../src/graph/BicliqueLite.h"
#include"../../src/graph/OrderedVertexSet.h"


int test_graph_can_be_added_to_biclique(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    g.print_graph();

    // Try to add inadmissible vertex
    BicliqueLite test_biclique( (std::vector<size_t>){0}, (std::vector<size_t>){1} );
    if (g.can_be_added_to_biclique(2, test_biclique)) {
        std::cout << "Inadmissible vertex (2) reported as being added to biclique:  " << test_biclique.to_string() << std::endl;
        error = true;
    }
    test_biclique = BicliqueLite((std::vector<size_t>){1}, (std::vector<size_t>){0} );
    if (g.can_be_added_to_biclique(2, test_biclique)) {
        std::cout << "Inadmissible vertex (2) reported as being added to biclique:  " << test_biclique.to_string() << std::endl;
        error = true;
    }

    // Try to add admissible vertex
    test_biclique = BicliqueLite( (std::vector<size_t>){0}, (std::vector<size_t>){1} );
    if (!g.can_be_added_to_biclique(3, test_biclique)) {
        std::cout << "Admissible vertex (3) reported as not being added to biclique:  " << test_biclique.to_string() << std::endl;
        error = true;
    }
    test_biclique = BicliqueLite( (std::vector<size_t>){1}, (std::vector<size_t>){0} );
    if (!g.can_be_added_to_biclique(3, test_biclique)) {
        std::cout << "Admissible vertex (3) reported as not being added to biclique:  " << test_biclique.to_string() << std::endl;
        error = true;
    }

    return error;

}
