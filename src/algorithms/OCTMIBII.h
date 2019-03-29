/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_OCTMIBII_H
#define BICLIQUES_OCTMIBII_H

#include <fstream>
#include <queue>
#include <vector>
#include <list>
#include <stack>
#include <ctime>
#include "../graph/BicliqueLite.h"
#include "../graph/Graph.h"
#include "../graph/OrderedVector.h"
#include "OutputOptions.h"
#include "MaximalCrossingBicliquesBipartite.h"
#include "AllMaximalIndependentSets.h"
#include "../graph/OctGraph.h"


/**
 * Edits the given biclique to be maximal with respect to expansion_set1.
 * Then, checks agains expansion_set2 to see if the biclique is maximal wrt set2.
 * Returns true if maximal wrt set2 (and edits the Biclique), false and does not edit otherwise.
 */
bool make_maximal(const Graph & g, BicliqueLite & b, OrderedVertexSet expansion_set1, OrderedVertexSet expansion_set2);

BicliqueLite add_to(const Graph & g, BicliqueLite & b, bool left, size_t vertex);

void octmibii_cc(OutputOptions & octmibii_results,
        const Graph & g,
        const OrderedVertexSet input_oct_set,
        const OrderedVertexSet input_left_set,
        const OrderedVertexSet input_right_set);

/**
 * Wrapper for our OCTMIB algorithm together that first separates out
 * connected components, runs our algorithm on each CC, and aggregates
 * the results.
 */
void octmibii(OutputOptions & octmibii_results,
             const Graph & g,
             OrderedVertexSet input_oct_set,
             OrderedVertexSet input_left_set,
             OrderedVertexSet input_right_set);


#endif //BICLIQUES_OCTMIBII_H
