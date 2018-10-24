/**
 * ExpansionResult bundles metadata about the outcome of calling BicliqueBlueprint::expand. See BicliqueBlueprint.cpp
 * for full documentation.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */

#ifndef BICLIQUES_EXPANSIONRESULT_H
#define BICLIQUES_EXPANSIONRESULT_H

#include "stddef.h"  // apparently required for size_t

class ExpansionResult {

public:
    enum RETURN_TYPE {SUCCESS, DISCONTINUE, BAN};
    ExpansionResult(RETURN_TYPE);
    ExpansionResult(RETURN_TYPE, size_t);

    const RETURN_TYPE & get_return_type() const;
    size_t get_vertex() const;

private:
    RETURN_TYPE return_type;
    size_t vertex;

};


#endif //BICLIQUES_EXPANSIONRESULT_H
