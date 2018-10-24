/**
 * ExpansionResult bundles metadata about the outcome of calling
 * BlueprintLite::expand. A result consists of a RETURN_TYPE, which is one
 * of {SUCCESS, DISCONTINUE, BAN}. If the result is BAN the vertex to ban
 * and the first node of the range that needs to be updated
 * (minimum_updated_node) should be set.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


// Include ExpansionResult header
#include "ExpansionResult.h"


/**
 * Construct a new ExpansionResult, setting only the return type. `vertex` and
 * `minimum_updated_node` will be undefined when using this constructor.
 *
 * @param return_type Return type of the ExpansionResult.
 */
ExpansionResult::ExpansionResult(RETURN_TYPE return_type) {

    this->return_type = return_type;

}


/**
 * Construct a new ExpansionResult, setting `return_type`,
 * `vertex`, and `minimum_updated_node`.
 *
 * @param return_type          Return type of the ExpansionResult.
 * @param vertex               Vertex to ban or delay.
 */
ExpansionResult::ExpansionResult(RETURN_TYPE return_type, size_t vertex) : ExpansionResult(return_type) {

    this->vertex = vertex;

}


/**
 * Get the ExpansionResult return type.
 *
 * @return return_type
 */
const ExpansionResult::RETURN_TYPE & ExpansionResult::get_return_type() const {
    return return_type;
}


/**
 * Get the ExpansionResult vertex.
 *
 * @return vertex
 */
size_t ExpansionResult::get_vertex() const {
    return vertex;
}
