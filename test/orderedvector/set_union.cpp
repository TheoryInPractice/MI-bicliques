/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/graph/OrderedVector.h"


int test_orderedvector_set_union(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    OrderedVector set_one((std::vector<size_t>){3, 5, 7, 9});

    // add nothing new
    OrderedVector set_two((std::vector<size_t>){3,5});

    // add something at the start
    OrderedVector set_three((std::vector<size_t>){1, 3, 5});

    // add things inbetween
    OrderedVector set_four((std::vector<size_t>){5, 6, 7, 8, 9});

    // add things at the end
    OrderedVector set_five((std::vector<size_t>){3, 5, 8, 10});

    // Check union with a subset
    OrderedVector new_set(set_one.set_union(set_two));
    if (!(set_one == new_set)) {
        std::cout << "Set union with a subset does not produce original set." << std::endl;
        error = true;
    }

    // Check union with a new, smaller element, 1
    OrderedVector true_union_three((std::vector<size_t>){1, 3, 5, 7, 9});
    OrderedVector computed_union_three(set_one.set_union(set_three));
    if (true_union_three.size() != computed_union_three.size()) {
        std::cout << "Set union with new, smaller element does not expand set." << std::endl;
        error = true;
    }
    for (size_t idx = 0; idx < computed_union_three.size(); idx++) {
        if (computed_union_three[idx] != true_union_three[idx]) {
            std::cout << "Set union with new, smaller element is wrong at element: " << true_union_three[idx] << std::endl;
            error = true;
        }
    }

    // Check union with elements inbetween
    OrderedVector true_union_four((std::vector<size_t>){3, 5, 6, 7, 8, 9});
    OrderedVector computed_union_four(set_one.set_union(set_four));
    if (true_union_four.size() != computed_union_four.size()) {
        std::cout << "Set union with new elements inbetween does not expand set." << std::endl;
        error = true;
    }
    for (size_t idx = 0; idx < true_union_four.size(); idx++) {
        if (computed_union_four[idx] != true_union_four[idx]) {
            std::cout << "Set union with new elements inbetween is wrong at element: " << true_union_four[idx] << std::endl;
            error = true;
        }
    }

    // Check union with elements at end
    OrderedVector true_union_five((std::vector<size_t>){3, 5, 7, 8, 9, 10});
    OrderedVector computed_union_five(set_one.set_union(set_five));
    if (true_union_five.size() != computed_union_five.size()) {
        std::cout << "Set union with new elements at end does not expand set." << std::endl;
        error = true;
    }
    for (size_t idx = 0; idx < true_union_five.size(); idx++) {
        if (computed_union_five[idx] != true_union_five[idx]) {
            std::cout << "Set union with new elements at end is wrong at element: " << true_union_five[idx] << std::endl;
            error = true;
        }
    }

    return error;

}
