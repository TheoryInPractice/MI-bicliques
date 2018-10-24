/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_MAXIMUMINDEPENDENTSET_H
#define BICLIQUES_MAXIMUMINDEPENDENTSET_H

#include <memory>
#include <vector>
#include "../graph/Graph.h"
#include "../graph/OrderedVertexSet.h"
#include "../graph/EditableGraph.h"


bool check_is_maximal(std::shared_ptr<Graph> graph_ptr,
                      OrderedVertexSet& independent_set);

void recursive_eppstein(std::shared_ptr<Graph> graph_ptr,
                        std::vector<std::vector<size_t>>& list_of_MIS,
                        EditableGraph &ed_graph,
                        std::unordered_map<size_t,bool> &independent_set,
                        size_t size_limit
                       );

std::vector<std::vector<size_t>> get_all_mis(Graph & g);

#endif //BICLIQUES_MAXIMUMINDEPENDENTSET_H
