/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel, Trey Woodlief
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_MICA_H
#define BICLIQUES_MICA_H


#include <vector>
#include <queue>
#include "../graph/BicliqueLite.h"
#include "../graph/BlueprintLite.h"
#include "../graph/ExpansionResult.h"
#include "../graph/Graph.h"
#include "../graph/OctGraph.h"
#include "../graph/SearchTreeBag.h"

#include "../algorithms/SimpleOCT.h"
#include "../algorithms/AllMaximalIndependentSets.h"

#include "OutputOptions.h"

std::vector<BicliqueLite*> consensus(BicliqueLite &, BicliqueLite &);
void mica_cc(OutputOptions &, const Graph &);
std::vector<BicliqueLite> mica(const Graph &);

/**
 * MICA algorithm that supplies the seed set of bicliques and the working set of bicliques, in that order.
 * If the working set is empty, it will be initialized to be the seed set.
 */
void mica_initialized(OutputOptions &, const Graph &, std::set<BicliqueLite>, std::set<BicliqueLite>);




#endif //BICLIQUES_MICA_H
