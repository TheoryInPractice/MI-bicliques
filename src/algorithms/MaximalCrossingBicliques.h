/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_MAXIMALCROSSINGBICLIQUES_H
#define BICLIQUES_MAXIMALCROSSINGBICLIQUES_H

#include <vector>
#include <list>

#include "../graph/Graph.h"
#include "../graph/BicliqueLite.h"
#include "../graph/BlueprintLite.h"
#include "../graph/SearchTreeBag.h"
#include "../graph/ExpansionResult.h"

#include "../graph/OrderedVector.h"


void update_iterative_pointers_in_blueprint(SearchTreeBag &);

void handle_bans(const size_t & blueprint_id,
                 const size_t & expanded_vertex,
                 std::list<SearchTreeBag> & bag_list);

void process_expansion_result(BlueprintLite & current_blueprint,
                              ExpansionResult & expand_result,
                              std::vector<BicliqueLite> & mib_list,
                              std::unordered_map<std::string,bool> & local_duplicates_list,
                              std::list<SearchTreeBag> & bag_list,
                              SearchTreeBag & next_bag);


std::vector<BicliqueLite> maximal_crossing_bicliques(const Graph &graph,
                            const OrderedVector &independent_set);

inline std::vector<BicliqueLite> maximal_crossing_bicliques(const Graph &graph,
                                     const std::vector<size_t> &independent_set) {
    OrderedVector temp_vec(independent_set);
    return maximal_crossing_bicliques(graph, temp_vec);
}

inline std::vector<BicliqueLite> maximal_crossing_bicliques(const Graph &graph,
                                     const OrderedVertexSet &independent_set) {

    return maximal_crossing_bicliques(graph, independent_set.get_vertices());
}

#endif //BICLIQUES_MAXIMALCROSSINGBICLIQUES_H
