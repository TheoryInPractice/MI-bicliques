/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_OCTMIB_H
#define BICLIQUES_OCTMIB_H


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

void process_expansion_result(OutputOptions & octmib_results,
                              BlueprintLite & current_blueprint,
                              ExpansionResult & expand_result,
                              std::unordered_map<std::string,bool> & global_duplicates_list,
                              std::unordered_map<std::string,bool> & local_duplicates_list,
                              std::vector<SearchTreeBag> & bag_list,
                              SearchTreeBag & next_bag);


void octmib_with_oct(OutputOptions & results, OctGraph &);


void octmib(OutputOptions & results,
             const Graph &,
             OrderedVertexSet input_oct_set = OrderedVertexSet(),
             OrderedVertexSet input_left_set = OrderedVertexSet(),
             OrderedVertexSet input_right_set = OrderedVertexSet() );


void octmib_cc(OutputOptions & results,
                const Graph &,
                OrderedVertexSet input_oct_set = OrderedVertexSet(),
                OrderedVertexSet input_left_set = OrderedVertexSet(),
                OrderedVertexSet input_right_set = OrderedVertexSet() );


std::vector<BicliqueLite> octmib(const Graph &,
                              OrderedVertexSet input_oct_set = OrderedVertexSet(),
                              OrderedVertexSet input_left_set = OrderedVertexSet(),
                              OrderedVertexSet input_right_set = OrderedVertexSet() );



#endif //BICLIQUES_OCTMIB_H
