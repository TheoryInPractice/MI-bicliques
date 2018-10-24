/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


#include <iostream>
#include <vector>
#include "../../src/graph/ExpansionResult.h"


/**
 * Test constructor for ExpansionResult with all possible return values.
 * Tests other functions of class as well, in order to check
 * the objects are constructed correctly.
 */
int test_expansionresult_constructor(int argc, char **argv) {

    // Whether or not an error has occurred.
    bool error = false;

    // constructor(RETURN_TYPE)
    // Construct an object and message for each return type
    std::vector<ExpansionResult::RETURN_TYPE> return_types = {
        ExpansionResult::RETURN_TYPE::SUCCESS,
        ExpansionResult::RETURN_TYPE::DISCONTINUE,
        ExpansionResult::RETURN_TYPE::BAN
    };
    std::vector<std::string> return_message = {
        "SUCCESS", "DISCONTINUE", "BAN"
    };

    for (size_t idx = 0; idx < return_types.size(); idx++) {

        ExpansionResult test_expansion_result(return_types[idx]);

        if (return_types[idx] != test_expansion_result.get_return_type()) {
            std::cout << "ExpansionResult failed to construct with "
                      << return_message[idx] << " return_type." << std::endl;
            error = true;
        }

    }

    ExpansionResult test_expansion_result(ExpansionResult::RETURN_TYPE::SUCCESS, 0);

    if (ExpansionResult::RETURN_TYPE::SUCCESS != test_expansion_result.get_return_type()) {
        std::cout << "ExpansionResult failed to construct with SUCCESS return_type." << std::endl;
        error = true;
    }

    size_t get_vertex = test_expansion_result.get_vertex();
    if (get_vertex != 0) {  // hard-coded value
        std::cout << "ExpansionResult failed to construct with vertex 0; has " << get_vertex << " instead." << std::endl;
        error = true;
    }

    test_expansion_result = ExpansionResult (ExpansionResult::RETURN_TYPE::SUCCESS, 0);

    if (ExpansionResult::RETURN_TYPE::SUCCESS != test_expansion_result.get_return_type()) {
        std::cout << "ExpansionResult failed to construct with SUCCESS return_type." << std::endl;
        error = true;
    }

    get_vertex = test_expansion_result.get_vertex();
    if (get_vertex != 0) {  // hard-coded value
        std::cout << "ExpansionResult failed to construct with vertex 0; has " << get_vertex << " instead." << std::endl;
        error = true;
    }

    return error;

}
