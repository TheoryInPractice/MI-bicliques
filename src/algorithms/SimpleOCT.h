/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_SIMPLEOCT_H
#define BICLIQUES_SIMPLEOCT_H

#include "../graph/OrderedVertexSet.h"
#include "../graph/Graph.h"


void simpleoct(const Graph & g,
                 OrderedVertexSet & oct_set,
                 OrderedVertexSet & left_partition,
                 OrderedVertexSet & right_partition
                );


#endif //BICLIQUES_SIMPLEOCT_H
