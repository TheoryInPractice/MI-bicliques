/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_SIMPLECCS_H
#define BICLIQUES_SIMPLECCS_H

#include "../graph/Graph.h"


std::vector<std::vector<size_t>> simpleccs(const Graph & g);


#endif //BICLIQUES_SIMPLECCS_H
