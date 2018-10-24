/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_MAXIMALCROSSINGBICLIQUESBIPARTITE_H
#define BICLIQUES_MAXIMALCROSSINGBICLIQUESBIPARTITE_H

#include <vector>
#include "../graph/Graph.h"
#include "../graph/OrderedVertexSet.h"
#include "../graph/BicliqueLite.h"
#include "../graph/SearchTreeBagPlus.h"

#include "OutputOptions.h"



void maximal_crossing_bicliques_bipartite(OutputOptions &mcbbresults,
                                          Graph &graph,
                                          const std::vector<size_t> &left_set,
                                          const std::vector<size_t> &right_set);


std::vector<BicliqueLite> maximal_crossing_bicliques_bipartite(Graph &graph,
                                                const std::vector<size_t> &left,
                                                const std::vector<size_t> &right);


#endif //BICLIQUES_MAXIMALCROSSINGBICLIQUESBIPARTITE_H
