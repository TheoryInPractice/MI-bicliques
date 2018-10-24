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


int test_graph_is_biclique(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Create graph
    std::string path_to_file = "./test/test_graph.txt";
    Graph g(path_to_file, Graph::FILE_FORMAT::adjlist);

    // Create false biclique: edge in one partition
    BicliqueLite test_biclique( (std::vector<size_t>){0,1}, (std::vector<size_t>){2} );
    if (g.is_biclique(test_biclique)) {
        std::cout << "Non-biclique reported as biclique:  " << test_biclique.to_string() << std::endl;
        std::cout << "non-biclique because partition " << vector_to_string(test_biclique.get_left());
        std::cout << " contains edge " << test_biclique.get_left()[0] << " " << test_biclique.get_left()[1] << std::endl;

        error = true;
    }

    // Create false biclique: edge missing between partitions
    test_biclique = BicliqueLite( (std::vector<size_t>){1,4}, (std::vector<size_t>){3,6} );
    if (g.is_biclique(test_biclique)) {
        std::cout << "Non-biclique reported as biclique:  " << test_biclique.to_string() << std::endl;
        std::cout << "non-biclique because edge 1,6 is not present."  << std::endl;

        error = true;
    }

    // Create true biclique
    test_biclique = BicliqueLite( (std::vector<size_t>){1,4}, (std::vector<size_t>){2,3} );
    if (!g.is_biclique(test_biclique)) {
        std::cout << "Biclique misreported as non-biclique:  " << test_biclique.to_string() << std::endl;

        error = true;
    }

    // Create false biclique: multiple copies of a vertex in left partition
    test_biclique = BicliqueLite( (std::vector<size_t>){1,1,1,4}, (std::vector<size_t>){2,3} );
    if (g.is_biclique(test_biclique)) {
        std::cout << "Non-biclique reported as biclique:  " << test_biclique.to_string() << std::endl;
        std::cout << "non-biclique because a vertex appears multiple times."  << std::endl;

        error = true;
    }

    // Create false biclique: multiple copies of a vertex in one partition
    test_biclique = BicliqueLite( (std::vector<size_t>){1,4}, (std::vector<size_t>){2,2,3,3} );
    if (g.is_biclique(test_biclique)) {
        std::cout << "Non-biclique reported as biclique:  " << test_biclique.to_string() << std::endl;
        std::cout << "non-biclique because a vertex appears multiple times."  << std::endl;

        error = true;
    }

    // Create false biclique: left partition empty
    test_biclique = BicliqueLite( (std::vector<size_t>){}, (std::vector<size_t>){2,3} );
    if (g.is_biclique(test_biclique)) {
        std::cout << "Non-biclique reported as biclique:  " << test_biclique.to_string() << std::endl;
        std::cout << "non-biclique because left partition is empty."  << std::endl;

        error = true;
    }

    // Create false biclique: right partition empty
    test_biclique = BicliqueLite( (std::vector<size_t>){1,4}, (std::vector<size_t>){} );
    if (g.is_biclique(test_biclique)) {
        std::cout << "Non-biclique reported as biclique:  " << test_biclique.to_string() << std::endl;
        std::cout << "non-biclique because right partition is empty."  << std::endl;

        error = true;
    }

    return error;

}
