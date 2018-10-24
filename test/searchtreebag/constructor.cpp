/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


#include <iostream>
#include <vector>
#include "../../src/graph/SearchTreeBag.h"


/**
 * Test that the SearchTreeBag constructor works.
 */
int test_searchtreebag_constructor(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // Left and right vertex sets
    size_t expanded_vertex = 0;
    SearchTreeBag test_bag(expanded_vertex);

    // Check size
    if ( test_bag.size() != 0 ) {
        std::cout << "Empty SearchTreeBag should have size 0, not " << test_bag.size() << std::endl;
        error = true;
    }

    // Check expanded vertex
    if ( test_bag.get_expanded_vertex() != expanded_vertex ) {
        std::cout << "Empty SearchTreeBag should have expanded_vertex = ";
        std::cout << expanded_vertex << ", not " << test_bag.get_expanded_vertex();
        std::cout << std::endl;
        error = true;
    }

    // Now test constructor with iterative sets
    std::vector<size_t> iterative_wait = {6,7,8};
    OrderedVertexSet iterative_in = (std::vector<size_t>) {3,4,5};
    std::vector<size_t> iterative_past = {0,1,2};
    expanded_vertex = 6;
    size_t last_iter_in_node = iterative_in.back();

    SearchTreeBag test_bag2(iterative_wait,
                            iterative_in,
                            iterative_past,
                            expanded_vertex);

    // Check iterative sets inside searchtreebag
    if (test_bag2.get_iterative_wait() != iterative_wait) {
        std::cout << "SearchTreeBag get_iterative_wait should return ";
        std::cout << vector_to_string(iterative_wait) << std::endl;
        std::cout << "not " << vector_to_string(test_bag2.get_iterative_wait());
        std::cout << std::endl;
        error = true;
    }
    if (test_bag2.get_iterative_in() != iterative_in) {
        std::cout << "SearchTreeBag get_iterative_in should return ";
        std::cout << iterative_in.to_string() << std::endl;
        std::cout << "not " << test_bag2.get_iterative_in().to_string();
        std::cout << std::endl;
        error = true;
    }
    if (test_bag2.get_iterative_past() != iterative_past) {
        std::cout << "SearchTreeBag get_iterative_past should return ";
        std::cout << vector_to_string(iterative_past) << std::endl;
        std::cout << "not " << vector_to_string(test_bag2.get_iterative_past());
        std::cout << std::endl;
        error = true;
    }
    // Check expanded vertex
    if ( test_bag2.get_expanded_vertex() != expanded_vertex ) {
        std::cout << "SearchTreeBag should have expanded_vertex = ";
        std::cout << expanded_vertex << ", not " << test_bag.get_expanded_vertex();
        std::cout << std::endl;
        error = true;
    }

    // Check pointers to iterative sets inside searchtreebag
    if (*test_bag2.get_iterative_wait_ptr() != iterative_wait) {
        std::cout << "SearchTreeBag get_iterative_wait_ptr should return ";
        std::cout << vector_to_string(iterative_wait) << std::endl;
        std::cout << "not " << vector_to_string(*test_bag2.get_iterative_wait_ptr());
        std::cout << std::endl;
        error = true;
    }
    if (*test_bag2.get_iterative_in_ptr() != iterative_in) {
        std::cout << "SearchTreeBag get_iterative_in_ptr should return ";
        std::cout << iterative_in.to_string() << std::endl;
        std::cout << "not " << test_bag2.get_iterative_in_ptr()->to_string();
        std::cout << std::endl;
        error = true;
    }
    if (*test_bag2.get_iterative_past_ptr() != iterative_past) {
        std::cout << "SearchTreeBag get_iterative_past_ptr should return ";
        std::cout << vector_to_string(iterative_past) << std::endl;
        std::cout << "not " << vector_to_string(*test_bag2.get_iterative_past_ptr());
        std::cout << std::endl;
        error = true;
    }

    // Check last node in iterative in
    if (test_bag2.get_last_iter_in_node() != last_iter_in_node) {
        std::cout << "SearchTreeBag should have last_iter_in_node = ";
        std::cout << last_iter_in_node << ", not " << test_bag2.get_last_iter_in_node();
        std::cout << std::endl;
        error = true;
    }

    return error;

}
