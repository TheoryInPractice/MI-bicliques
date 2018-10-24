/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include<iostream>
#include<vector>
#include"../../src/graph/OrderedVertexSet.h"


int test_orderedvertexset_range_bounds(int argc, char ** argv) {

    // Whether or not an error has occurred.
    bool error = false;

    std::unordered_map<size_t, bool> map = {
        {10, true},
        {23, true},
        {37, true},
        {55, true}
    };
    OrderedVertexSet ovs(map);

    size_t lower_true = 10;
    size_t lower_less = 9;
    size_t lower_more = 11;

    size_t upper_true = 55;
    size_t upper_less = 51;
    size_t upper_more = 65;

    // test front
    if ( ovs.front() != lower_true ) {
        std::cout << "lower bound should be " << lower_true << " not " << ovs.front() << std::endl;
        error = true;
    }

    // test back
    if ( ovs.back() != upper_true ) {
        std::cout << "upper bound should be " << upper_true << " not " << ovs.back() << std::endl;
        error = true;
    }

    // test contains_range
    // Should exclude these:
    std::vector<std::vector<size_t>> test_cases = {
                                                    {lower_less, upper_less},
                                                    {lower_more, upper_more},
                                                };
    for (auto dummy_range : test_cases) {
        if ( ovs.contains_range(dummy_range[0], dummy_range[1]) ) {
            std::cout << "Range of " << lower_true << ", " << upper_true << " should not include " << dummy_range[0] << ", " << dummy_range[1] << std::endl;
            error = true;
        }
    }

    // Should include these:
    test_cases = {
                    {lower_true, upper_true},
                    {lower_more, upper_less},
                };
    for (auto dummy_range : test_cases) {
        if ( !ovs.contains_range(dummy_range[0], dummy_range[1]) ) {
            std::cout << "Range of " << lower_true << ", " << upper_true << " should include range " << dummy_range[0] << ", " << dummy_range[1] << std::endl;
            error = true;
        }
    }


    // test disjoint_from_range
    test_cases = {
                    {lower_less, upper_less},
                    {lower_more, upper_more},
                    {lower_less, lower_true},
                    {upper_true, upper_more}
                };
    for (auto dummy_range : test_cases) {
        if ( ovs.disjoint_from_range(dummy_range[0], dummy_range[1]) ) {
            std::cout << "Range of " << lower_true << ", " << upper_true << " intersects " << dummy_range[0] << ", " << dummy_range[1] << std::endl;
            error = true;
        }
    }

    test_cases = {
                    {upper_more, upper_more + 10},
                    {lower_less - 1, lower_less}
                };
    for (auto dummy_range : test_cases) {
        if ( !ovs.disjoint_from_range(dummy_range[0], dummy_range[1]) ) {
            std::cout << "Range of " << lower_true << ", " << upper_true << " should not intersect " << dummy_range[0] << ", " << dummy_range[1] << std::endl;
            error = true;
        }

    }

    return error;

}
