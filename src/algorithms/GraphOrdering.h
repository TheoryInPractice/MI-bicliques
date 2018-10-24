/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* Simple algorithm for re-ordering nodes by degree.
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_GRAPHORDERING_H
#define BICLIQUES_GRAPHORDERING_H

#include "../graph/Graph.h"


inline std::vector<size_t> degree_reordering( const Graph & graph ) {

    // create sorted vector of node degrees
    std::vector<std::tuple<size_t,size_t>> degree_vector(graph.get_num_vertices());
    for (size_t idx = 0; idx < graph.get_num_vertices(); idx++) {
        degree_vector[idx] =  std::make_tuple( idx, graph.get_neighbors_vector(idx).size() );
    }

    std::sort(degree_vector.begin(),
              degree_vector.end(),
              [](std::tuple<size_t,size_t> a, std::tuple<size_t,size_t> b) {
                return std::get<1>(a) < std::get<1>(b);  // smallest degree
                }
             );

    // extract node indices from sorted vector of tuples
    std::vector<size_t> res(degree_vector.size());
    for (size_t idx = 0; idx < degree_vector.size(); idx++) {
        res[idx] = std::get<0>(degree_vector[idx]);
    }

    return res;

};

#endif //BICLIQUES_GRAPHORDERING_H
