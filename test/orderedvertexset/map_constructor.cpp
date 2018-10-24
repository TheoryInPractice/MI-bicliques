/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../../src/graph/OrderedVertexSet.h"

int test_orderedvertexset_map_constructor(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    std::unordered_map<size_t, bool> map = {
        {1, true},
        {2, false},
        {3, true},
        {7, true},
        {5, true}
    };
    size_t expected_size = 4;

    OrderedVertexSet s(map);

    if (s.size() != expected_size) {
        std::cout << "Set size does not match input array." << std::endl;
        error = true;
    }

    for (auto v :s) {

        if (map.find(v) == map.end() || !map[v]) {
            std::cout << "Vertex " << v << " should not be in the set." << std::endl;
            error = true;
        }

    }

    return error;

}
