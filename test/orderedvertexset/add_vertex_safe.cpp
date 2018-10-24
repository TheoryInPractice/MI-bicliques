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


int test_orderedvertexset_add_vertex_safe(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    OrderedVertexSet set_true((std::vector<size_t>) {1,3,5,7,9,10,13,15});
    std::vector<size_t> starting_vector = {3,9,13};
    OrderedVertexSet set_computed(starting_vector);

    std::vector<size_t> stuff_to_add = {10,5,15,7,1};

    for (auto v: stuff_to_add) {
        set_computed.add_vertex_safe(v);

        starting_vector.push_back(v);
        std::sort(starting_vector.begin(),starting_vector.end());

        OrderedVertexSet this_set_true(starting_vector);

        if (set_computed != this_set_true) {
            std::cout << "Sets do not match; did not safely add vertex " << v << std::endl;
            error = true;
        }

    }

    if (set_computed != set_true) {
        std::cout << "Full sets do not match." << std::endl;
        error = true;
    }

    // Now try adding something already there
    // OrderedVertexSet set_true((std::vector<size_t>) {1,3,5,7,9,10,13,15});
    std::unordered_map<size_t,bool> starting_map = {
                                                    {3, true},
                                                    {9, true},
                                                    {13, true}
                                                   };
    OrderedVertexSet set_computed2(starting_map);

    std::vector<size_t> stuff_to_add2 = {3,10,1,10,5,9,15,15,7,1,7};

    for (auto v: stuff_to_add2) {
        set_computed2.add_vertex_safe(v);

        starting_map[v] = true;
        OrderedVertexSet this_set_true(starting_map);

        if (set_computed2 != this_set_true) {
            std::cout << "Sets do not match; did not safely add from map on vertex " << v << std::endl;

            std::cout << "True: ";
            for (auto v: this_set_true) std::cout << v << " ";
            std::cout << std::endl;

            std::cout << "Computed: ";
            for (auto v: set_computed2) std::cout << v << " ";
            std::cout << std::endl;

            error = true;
        }

    }

    return error;

}
