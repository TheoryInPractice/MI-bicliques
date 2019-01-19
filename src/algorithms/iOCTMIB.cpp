/**
* This implements iOCTMIB, our improved version of OCTMIB
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*
* NOTE:
*   This code assumes no node is isolated.
*/

#include "iOCTMIB.h"
#include "SimpleCCs.h"

/**
 * Edits the given biclique to be maximal with respect to expansion_set1.
 * Then, checks agains expansion_set2 to see if the biclique is maximal wrt set2.
 * Returns true if maximal wrt set2 (and edits the Biclique), false and does not edit otherwise.
 */
bool make_maximal(const Graph & g, BicliqueLite & b, OrderedVertexSet expansion_set1, OrderedVertexSet expansion_set2) {
    // Proceed with update set_x and set_y.
    OrderedVector set_x = OrderedVector(b.get_left());
    OrderedVector set_y = OrderedVector(b.get_right());
    for (auto current_vertex : expansion_set1) {
    	if (g.is_completely_independent_from(current_vertex, set_x) && g.is_completely_connect_to(current_vertex, set_y)) {
            set_x.bubble_sort(current_vertex);
        }

        if (g.is_completely_independent_from(current_vertex, set_y) && g.is_completely_connect_to(current_vertex, set_x)) {
            set_y.bubble_sort(current_vertex);
        }

    }
    for (auto current_vertex : expansion_set2) {
    	if (g.is_completely_independent_from(current_vertex, set_x) && g.is_completely_connect_to(current_vertex, set_y)) {
            return false;
        }

        if (g.is_completely_independent_from(current_vertex, set_y) && g.is_completely_connect_to(current_vertex, set_x)) {
            return false;
        }

    }
    b = BicliqueLite(set_x, set_y);
    return true;
}

BicliqueLite add_to(const Graph & g, BicliqueLite & b, bool left, size_t vertex) {
	OrderedVector x_edited = b.get_left();
	OrderedVector y_edited = b.get_right();
	if (left) { //add to x
		x_edited.vector_minus_neighborhood(g, vertex);
		x_edited.bubble_sort(vertex);
		y_edited.vector_intersect_neighborhood(g, vertex);
	} else { //add to y
		y_edited.vector_minus_neighborhood(g, vertex);
		y_edited.bubble_sort(vertex);
		x_edited.vector_intersect_neighborhood(g, vertex);
	}
	return BicliqueLite(x_edited, y_edited);
}

void ioctmib_cc(OutputOptions & ioctmib_results,
        const Graph & g,
        const OrderedVertexSet input_oct_set,
        const OrderedVertexSet input_left_set,
        const OrderedVertexSet input_right_set) {

	std::set<BicliqueLite> hash_set;
	std::stack<BicliqueLite> stack;
	// Step (2) - generate Bicliques based on the MIS from the neighborhoods of the OCT set.
	OrderedVertexSet left_right = input_left_set.set_union(input_right_set);
	for (auto octitr = input_oct_set.begin(); octitr != input_oct_set.end(); octitr++) {
		auto neighborhood = g.subgraph(g.get_neighbors_vector(*octitr));
		std::vector<std::vector<size_t>> mis_list = get_all_mis(neighborhood);
		for (auto mis_itr = mis_list.begin(); mis_itr != mis_list.end(); mis_itr++) {
			BicliqueLite b = BicliqueLite(std::vector<size_t>{*octitr}, *mis_itr);
			if (make_maximal(g, b, input_oct_set, left_right)) {
				if (hash_set.insert(b).second) {
					stack.push(b);
				}
			}
		}
	}
	// Step (1) - call MCBB on the bipartite portion
	OutputOptions temp;
	maximal_crossing_bicliques_bipartite(temp, g, input_left_set.get_vertices(), input_right_set.get_vertices());

	for (auto b_itr = temp.mibs_computed.begin(); b_itr != temp.mibs_computed.end(); b_itr++) {
		BicliqueLite b = *b_itr;
		make_maximal(g, b, input_oct_set, OrderedVertexSet());
		if (hash_set.insert(b).second) {
			stack.push(b);
			ioctmib_results.push_back(b);
		}
	}
	//we now have a stack and dictionary that contains all bicliques we have found up until now
	while (!stack.empty()) {
		BicliqueLite cur = stack.top();
		stack.pop(); //pop does not return in c++, must do top then pop
		for (auto o_itr = input_oct_set.begin(); o_itr != input_oct_set.end(); o_itr++) {
			//this could be more efficient if we used sets within BicliqueLite
			if (find(cur.get_left().begin(), cur.get_left().end(), *o_itr) == cur.get_left().end() && find(cur.get_right().begin(), cur.get_right().end(), *o_itr) == cur.get_right().end()) {
				BicliqueLite m1 = add_to(g, cur, true, *o_itr);
				make_maximal(g, m1, input_oct_set, OrderedVertexSet());
				if (hash_set.insert(m1).second) {
					stack.push(m1);
					ioctmib_results.push_back(m1);
				}
				BicliqueLite m2 = add_to(g, cur, false, *o_itr);
				make_maximal(g, m2, input_oct_set, OrderedVertexSet());
				if (hash_set.insert(m2).second) {
					stack.push(m2);
					ioctmib_results.push_back(m2);
				}
			}
		}
	}
}
