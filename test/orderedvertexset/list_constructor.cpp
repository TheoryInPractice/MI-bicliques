/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


#include <iostream>
#include <vector>
#include "../../src/graph/OrderedVertexSet.h"


int test_orderedvertexset_list_constructor(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    std::vector<size_t> true_vector = {1, 3, 5, 7, 9};
    OrderedVertexSet s(true_vector);

    if (s.size() != true_vector.size()) {
        std::cout << "Set size does not match input array." << std::endl;
        error = true;
    }

    for (size_t vertex : true_vector) {
        if (!s.has_vertex(vertex)) {
            std::cout << "Set does not contain vertex: " << vertex << std::endl;
            error = true;
        }
    }

    return error;

}
