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
#include <algorithm>

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
		if (g.get_neighbors(v).size() == 0) {
			continue;
		}
		std::vector<size_t> left, right, temp;
		left = g.get_neighbors_vector(v);
		std::cout << "Neighbors vector for " << v << " has size " << left.size() << " and elements: ";
		for (auto itr = left.begin(); itr != left.end(); itr++) {
			std::cout << *itr << " ";
		}
		std::cout << std::endl;
		right = g.get_neighborhood_intersection(left, false);
		//if right does not have the original vertex, add it in (should not be necessary)	
		if (find(right.begin(), right.end(), v) == left.end()) {
			right.push_back(v);
		}
		auto it = C0.insert(BicliqueLite(left, right));
		std::cout << "before putting in biclique object, left and right are:" << std::endl;
		for (auto itr = left.begin(); itr != left.end(); itr++) {
			std::cout << *itr << " ";
		}
		std::cout << std::endl;
		for (auto itr = right.begin(); itr != right.end(); itr++) {
			std::cout << *itr << " ";
		}
		std::cout << std::endl;
		std::cout << "For starting vertex: " << v << " found max'l biclique (" << g.is_biclique(*(it.first)) << "):" << std::endl;
		std::cout << g.biclique_string(*(it.first)) << std::endl; 
	}
	mica_initialized(mica_results, g, C0, std::set<BicliqueLite>());
}

void mica_initialized(OutputOptions & mica_results, const Graph & g, std::set<BicliqueLite> C0, std::set<BicliqueLite> C) {
	//if the working set is empty, initialize to be the seed set.
	if (C.empty()) {
		std::copy(C0.begin(), C0.end(), std::inserter(C, C.begin()));
		for (auto b: C0) {
			mica_results.push_back(b);
		}
	}
	bool found = true;
	while (found) {
		found = false;
		for (std::set<BicliqueLite>::iterator itc0 = C0.begin(); itc0 != C0.end(); itc0++) {
			for (std::set<BicliqueLite>::iterator itc = C.begin(); itc != C.end(); itc++) {
				std::cout << "Consensus of :" << std::endl;
				std::cout << itc0->to_string() << std::endl;
				std::cout << itc->to_string() << std::endl;
				std::cout << "Found:" << std::endl;
				std::vector<BicliqueLite*> cons = consensus(*itc0, *itc);
				for (std::vector<BicliqueLite*>::iterator itb = cons.begin(); itb != cons.end(); itb++) {
					//extend the bicliques that we found by taking the set intersections of the sides
					std::vector<size_t> left, right;
					std::cout << g.biclique_string(*(*itb));
					right = g.get_neighborhood_intersection((*itb)->get_left(), false);
					left = g.get_neighborhood_intersection(right, false);
					//left = (*itb)->get_left();
					//right = (*itb)->get_right();
					BicliqueLite ext(left, right);
					BicliqueLite ext_rev(right, left);
					// need to check both sides
					if (C.find(ext) == C.end() && C.find(ext_rev) == C.end()) {
						found = true; //we have found a vector we do not have already
						std::cout << " - added";
						C.insert(ext);
						mica_results.push_back(ext);
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
	}
	//std::copy(C.begin(), C.end(), back_inserter(mica_results.mibs_computed));
//	mica_results.bipartite_num_mibs = C.size();
	for (auto & b : C) {
		std::cout << g.biclique_string(b) << std::endl;
	}
}

/**
 * Wrapper for our OCTMIB algorithm together that first separates out
 * connected components, runs our algorithm on each CC, and aggregates
 * the results.
 */
void mica(OutputOptions & mica_results,
             const Graph & g) {

    //std::cout << "in mica" << std::endl;
    mica_results.set_base_graph(g);
    mica_results.n = g.get_num_vertices();
    mica_results.m = g.get_num_edges();

    // Determine connected components
    clock_t begint = std::clock();
    std::vector<std::vector<size_t>> vector_of_ccs = simpleccs(g);
    clock_t endt = std::clock();
    mica_results.time_ccs += double(endt - begint) / CLOCKS_PER_SEC;
    mica_results.num_connected_components = vector_of_ccs.size();

    std::cout << "# Graph has " << vector_of_ccs.size();
    std::cout << " connected components. CC ran in " << mica_results.time_ccs;
    std::cout << std::endl;

    // Get correct number edges in prescribed OCT decomp, if provided
    if (input_oct_set.size() > 0) {
        Graph temp_graph = g.subgraph(input_oct_set);
        mica_results.num_oct_edges_given = temp_graph.get_num_edges();
    }


    if (vector_of_ccs.size() > 1) {

        // run octmib on each CC
        for (size_t which_cc = 0; which_cc < vector_of_ccs.size(); which_cc++) {

            // Time this CC
            begint = std::clock();

            std::vector<size_t> vertex_subset = vector_of_ccs[which_cc];
            mica_results.relabeling_mode = false;

            // relabel ground truth using this ordering
            std::vector<size_t> reverse_ordering(g.get_num_vertices());
            for (size_t idx1=0; idx1<vertex_subset.size(); idx1++) {
                reverse_ordering[vertex_subset[idx1]] = idx1;
            }

            std::cout << "# CC " << which_cc + 1 << " of ";
            std::cout << vector_of_ccs.size() << std::endl;
            std::cout << "#\tsize: " << vertex_subset.size() << "/";
            std::cout << g.get_num_vertices() << std::endl;

            // Skip Isolated vertices and empty sets
            if (vertex_subset.size() <= 1) {
                mica_results.isolates ++;
                continue;
            }

            // Isolated edges are MIBs
            if (vertex_subset.size() == 2) {
                BicliqueLite temp((std::vector<size_t>){vertex_subset.front()},
                                  (std::vector<size_t>){vertex_subset.back()});
                mica_results.push_back_bipartite(temp);
                mica_results.size_left ++;
                mica_results.size_right ++;
                continue;
            }

            Graph g_cc = g.subgraph(vertex_subset);

            mica_results.turn_on_relabeling_mode(vertex_subset);
            mica_cc(mica_results, g_cc);

            endt = std::clock();
            std::cout << "# this CC ran in ";
            std::cout << double(endt - begint) / CLOCKS_PER_SEC;
            std::cout << "\n#\n# " << std::endl;

        }

    }
    else {
        mica_cc(mica_results, g);
    }

    mica_results.total_num_mibs += mica_results.bipartite_num_mibs;
    mica_results.close_results();

}
