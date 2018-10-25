/**
 * BicliqueLite is a container class that tracks and manages vertices which
 * comprise some BicliqueLite in a graph.
 * See BicliqueLite.cpp for full documentation.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


#ifndef BICLIQUES_BICLIQUELITE_H
#define BICLIQUES_BICLIQUELITE_H


#include <sstream>
#include <string>
#include "OrderedVertexSet.h"

class BicliqueLite {

private:
    size_t num_vertices;
    std::vector<size_t> left, right, all;
    std::string biclique_string;

public:
    BicliqueLite();
    BicliqueLite(std::vector<size_t> left, std::vector<size_t> right);
    // overloading to work with OrderedVector and OrderedVertexSet
    template<class T>
    BicliqueLite(T left, T right) : left(left.get_vertices()), right(right.get_vertices()) {
        num_vertices = left.size() + right.size();
    }

    const std::vector<size_t> &get_left() const;
    const std::vector<size_t> &get_right() const;
    const std::vector<size_t> get_all_vertices() const;
    const std::vector<size_t> &get_all_vertices_persistent() ;
    bool operator==(const BicliqueLite &other) const;
    bool operator!=(const BicliqueLite &other) const;
    bool operator<(const BicliqueLite &other) const;
    inline bool operator>(const BicliqueLite &other) const {
        return ( !(*this<other) && !(*this==other) );
    }

    const std::string to_string();
    const std::string & to_string_persistent();
    inline size_t size() const {return num_vertices;};

};

/**
 * Some BicliqueLites are computed using labels of subgraph.
 * Must convert each BicliqueLite to use labelling of graph.
 */
 template <class T>
 std::vector<BicliqueLite> convert_node_labels_biclique(const std::vector<BicliqueLite> & list_of_BicliqueLites,
                                                    const T & label_converter){

    std::vector<BicliqueLite> res;
    for (const BicliqueLite & this_BicliqueLite: list_of_BicliqueLites) {

        std::vector<size_t> left = convert_node_labels_vector(this_BicliqueLite.get_left(), label_converter);
        std::vector<size_t> right = convert_node_labels_vector(this_BicliqueLite.get_right(), label_converter);

        res.push_back( BicliqueLite(left, right) );
    }

    return res;
}


/**
 * Overloading the above function. Relabel the vector of bicliques in place.
 */
 template <class T>
 void convert_node_labels_biclique(std::vector<BicliqueLite> & list_of_BicliqueLites,
                                                    const T & label_converter){

    for (size_t idx = 0; idx < list_of_BicliqueLites.size(); idx++ ) {

        BicliqueLite this_BicliqueLite = list_of_BicliqueLites[idx];

        std::vector<size_t> left = convert_node_labels_vector(this_BicliqueLite.get_left(), label_converter);
        std::vector<size_t> right = convert_node_labels_vector(this_BicliqueLite.get_right(), label_converter);

        list_of_BicliqueLites[idx] = BicliqueLite(left, right);
    }

}

/**
 * Overloading the above function. Relabel the vector of bicliques in place.
 */
 template <class T>
 void convert_node_labels_biclique(BicliqueLite & this_BicliqueLite,
                                   const T & label_converter){

    std::vector<size_t> left = convert_node_labels_vector(this_BicliqueLite.get_left(), label_converter);
    std::vector<size_t> right = convert_node_labels_vector(this_BicliqueLite.get_right(), label_converter);

    this_BicliqueLite = BicliqueLite(left, right);

}


#endif //BICLIQUES_BICLIQUELITE_H
