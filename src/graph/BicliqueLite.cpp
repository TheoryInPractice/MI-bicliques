/**
 * BicliqueLite is a container class that tracks and manages vertices which comprise some BicliqueLite in a graph. It adds
 * convenience functions for accessing data members and performing comparison and serialization. It does not guarantee
 * correctness. It is the responsibility of the caller to construct valid BicliqueLites with respect to the source graph.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


// Include BicliqueLite header
#include "BicliqueLite.h"
#include <iostream>
#include <algorithm>  // for std::merge


/**
 * Empty constructor.
 */
BicliqueLite::BicliqueLite() {

    num_vertices = 0;
    left = std::vector<size_t> {};
    right = std::vector<size_t> {};
    all = std::vector<size_t> {};
    biclique_string = std::string("");
}


/**
 * Construct a new BicliqueLite from two sets of vertices, left and right. The total size of the BicliqueLite is computed as
 * the combined size of the left and right sets.
 *
 * @param left  The first set of vertices that comprise the BicliqueLite.
 * @param right The second set of vertices which comprise the BicliqueLite.
 */
BicliqueLite::BicliqueLite(std::vector<size_t> left, std::vector<size_t> right) : left(left), right(right) {

    num_vertices = left.size() + right.size();
    all = std::vector<size_t> {};
}


/**
 * Get the vector being tracked as the left set of vertices.
 *
 * @return left
 */
const std::vector<size_t> & BicliqueLite::get_left() const {

    return left;

}


/**
 * Get the vector being tracked as the right set of vertices.
 *
 * @return right
 */
const std::vector<size_t> & BicliqueLite::get_right() const {

    return right;

}


/**
 * Return an vector containing all vertices from both the left and right sets.
 * Computed using merge sort.
 *
 * @return All vertices in the BicliqueLite.
 */
const std::vector<size_t> & BicliqueLite::get_all_vertices_persistent() {

    // merge
    if (all.size() == 0) {
        std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(all));
    }

    return all;

}

const std::vector<size_t> BicliqueLite::get_all_vertices() const {

    // merge
    std::vector<size_t> dummy = {};
    std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(dummy));

    return dummy;
}


/**
 * Compare this BicliqueLite to another BicliqueLite `b`, checking for equality. Two BicliqueLites are considered equal if and only
 * if they contain exactly the same set of vertices. We first check to verify that the BicliqueLites are of the same size
 * and quit early if they are not. Otherwise, we verify that all vertices of one BicliqueLite are contained within the set
 * of vertices of the other. We return false the moment a single vertex is found to be missing.
 *
 * @param  b The BicliqueLite that `this` will be compared to.
 * @return   True if the BicliqueLites are the same, false if they are not.
 */
bool BicliqueLite::operator==(const BicliqueLite &b) const {

    if (size() != b.size()) return false;

    // avoid copying partitions.
    const std::vector<size_t> &b_first = b.get_left();
    const std::vector<size_t> &b_second = b.get_right();

    if ((this->left == b_first) && (this->right == b_second)) return true;
    else if ((this->right == b_first) && (this->left == b_second)) return true;

    return false;

}


/**
 * Compare this BicliqueLite to another to determine if they are not equal. Equality is determined by deferring to the
 * BicliqueLite::operator==.
 *
 * @param  b The Biclqique that `this` will be compared to.
 * @return   True if the BicliqueLites are different, false if they are not.
 */
bool BicliqueLite::operator!=(const BicliqueLite &b) const {
    return !(*this == b);
}


/**
 * Compare this BicliqueLite to another BicliqueLite `b`, checking if this < b
 * according to standard lexicographic ordering.
 * Return false at first sign of a difference in ordering.
 *
 * @param  b The BicliqueLite that `this` will be compared to.
 * @return   True if this < b, false otherwise.
 */
bool BicliqueLite::operator<(const BicliqueLite &b) const {

    // Using pointers, references to avoid copying partitions.
    // First find smaller partition in each BicliqueLite
    const std::vector<size_t>* b_smaller = &b.get_left();
    const std::vector<size_t>* b_larger = &b.get_right();
    if (*b_larger < *b_smaller) {
        b_smaller = &b.get_right();
        b_larger = &b.get_left();
    }

    const std::vector<size_t>* this_larger = &(this->left);
    const std::vector<size_t>* this_smaller = &(this->right);
    if (this->left < this->right) {
        this_larger = &(this->right);
        this_smaller = &(this->left);
    }

    // Compare each BicliqueLite's smaller partition
    if (*this_smaller < *b_smaller) return true;

    // if !(this_smaller < b_smaller), and not equal then false
    if (*this_smaller != *b_smaller) return false;

    // Now we know this_smaller == b_smaller, so test larger.
    if (*this_larger < *b_larger) return true;

    return false;

}


/**
 * Convert a BicliqueLite to a string representation. A BicliqueLite is
 * represented as a string as a comma separated list of the vertices that it
 * contains. Vertices are sorted in lexicographic order (i.e., ascending order).
 *
 * NOTE: this function stores the sorted vector ALL internally,
 * as well as the string representation. This is only useful
 * in an implementation that will reuse the biclique object a lot.
 * Otherwise, this is memory-hungry.
 *
 * @return String representation of the BicliqueLite.
 */
const std::string & BicliqueLite::to_string_persistent() {

    // merge
    if (all.size() == 0) get_all_vertices_persistent();

    if (biclique_string.size() == 0) {
        std::stringstream s;
        for (size_t v : all) {
            s << v << ",";
        }
        biclique_string = s.str();
    }

    return biclique_string;

}


// Same output, but without saving anything internally
const std::string BicliqueLite::to_string() {

    auto dummy = get_all_vertices();
    std::stringstream s;
    for (size_t v : dummy) s << v << ",";

    return s.str();

}
