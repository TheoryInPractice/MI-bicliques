/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel, Trey Woodlief
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#include <ctime> // for timing
#include <csignal> // for triggering early termination
#include <list>

#include "MICA.h"
#include "MaximalCrossingBicliques.h"
#include "MaximalCrossingBicliquesBipartite.h"
#include "SimpleCCs.h"

#include "../graph/OrderedVector.h"


std::vector<BicliqueLite*> consensus(BicliqueLite b1, BicliqueLite b2) {
	std::vector<BicliqueLite*> bicliques;
	std::vector<size_t> left1, right1,
	                    left2, right2,
						left3, right3,
						left4, right4;
	// (X1 \cap X2, Y1 U Y2)
	std::set_intersection(b1.get_left().begin(), b1.get_left().end(),
				         b2.get_left().begin(), b2.get_left().end(),
						 std::back_inserter(left1));
	std::set_union(b1.get_right().begin(), b1.get_right().end(),
				         b2.get_right().begin(), b2.get_right().end(),
						 std::back_inserter(right1));
	// (X1 U X2, Y1 \cap Y2)
	std::set_union(b1.get_left().begin(), b1.get_left().end(),
				         b2.get_left().begin(), b2.get_left().end(),
						 std::back_inserter(left2));
	std::set_intersection(b1.get_right().begin(), b1.get_right().end(),
				         b2.get_right().begin(), b2.get_right().end(),
						 std::back_inserter(right2));
	// (Y1 U X2, X1 \cap Y2)
	std::set_union(b1.get_right().begin(), b1.get_right().end(),
			         	 b2.get_left().begin(), b2.get_left().end(),
					 	 std::back_inserter(left3));
	std::set_intersection(b1.get_left().begin(), b1.get_left().end(),
			         	 b2.get_right().begin(), b2.get_right().end(),
						 std::back_inserter(right3));
	// (X1 U Y2, Y1 \cap X2)
	std::set_union(b1.get_left().begin(), b1.get_left().end(),
			         	 b2.get_right().begin(), b2.get_right().end(),
					 	 std::back_inserter(left4));
	std::set_intersection(b1.get_right().begin(), b1.get_right().end(),
			         	 b2.get_left().begin(), b2.get_left().end(),
						 std::back_inserter(right4));

	if (left1.size() > 0 && right1.size() > 0) {
		bicliques.push_back(new BicliqueLite(left1, right1));
	}
	if (left2.size() > 0 && right2.size() > 0) {
		bicliques.push_back(new BicliqueLite(left2, right2));
	}
	if (left3.size() > 0 && right3.size() > 0) {
		bicliques.push_back(new BicliqueLite(left3, right3));
	}
	if (left4.size() > 0 && right4.size() > 0) {
		bicliques.push_back(new BicliqueLite(left4, right4));
	}
	return bicliques;
}

std::vector<BicliqueLite> mica(const Graph & g) {

    OutputOptions mica_results;

    mica_cc(mica_results, g);

    return mica_results.mibs_computed;

}

void mica_cc(OutputOptions & mica_results, const Graph & g) {
	// initialize the set of Bicliques that come from the stars of G. This covers G.
	std::set<BicliqueLite> C0;
	size_t num_vertices = g.get_num_vertices();
	for (size_t v = 0; v < num_vertices; v++) {
		std::vector<size_t> left, right, temp;
		right = g.get_neighbors_vector(v);
		left = g.get_neighborhood_intersection(right, false);
		left.push_back(v);
		C0.insert(BicliqueLite(left, right));
	}
	mica_initialized(mica_results, g, C0, std::set<BicliqueLite>());
}

void mica_initialized(OutputOptions & mica_results, const Graph & g, std::set<BicliqueLite> C0, std::set<BicliqueLite> C) {
	//if the working set is empty, initialize to be the seed set.
	if (C.empty()) {
		std::copy(C0.begin(), C0.end(), std::inserter(C, C.begin()));
	}
	bool found = true;
	while (found) {
		found = false;
		for (std::set<BicliqueLite>::iterator itc0 = C0.begin(); itc0 != C0.end(); itc0++) {
			for (std::set<BicliqueLite>::iterator itc = C.begin(); itc != C.end(); itc++) {
				std::vector<BicliqueLite*> cons = consensus(*itc0, *itc);
				for (std::vector<BicliqueLite*>::iterator itb = cons.begin(); itb != cons.end(); itb++) {
					//extend the bicliques that we found by taking the set intersections of the sides
					std::vector<size_t> left, right;
					//do we get two different if we do the other way
					right = g.get_neighborhood_intersection((*itb)->get_left(), false);
					left = g.get_neighborhood_intersection((*itb)->get_right(), false);
					BicliqueLite ext(left, right);
					if (C.find(ext) != C.end()) {
						found = true; //we have found a vector we do not have already
						C.insert(ext);
					}
				}
			}
		}
	}
	std::copy(C.begin(), C.end(), back_inserter(mica_results.mibs_computed));
}

