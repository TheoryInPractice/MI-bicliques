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

/**
 * Wrapper for our OCTMIB algorithm together that first separates out
 * connected components, runs our algorithm on each CC, and aggregates
 * the results.
 */
void octmib(OutputOptions & ioctmib_results,
             const Graph & g,
             OrderedVertexSet input_oct_set,
             OrderedVertexSet input_left_set,
             OrderedVertexSet input_right_set) {


    ioctmib_results.set_base_graph(g);
    ioctmib_results.n = g.get_num_vertices();
    ioctmib_results.m = g.get_num_edges();

    // Determine connected components
    clock_t begint = std::clock();
    std::vector<std::vector<size_t>> vector_of_ccs = simpleccs(g);
    clock_t endt = std::clock();
    ioctmib_results.time_ccs += double(endt - begint) / CLOCKS_PER_SEC;
    ioctmib_results.num_connected_components = vector_of_ccs.size();

    std::cout << "# Graph has " << vector_of_ccs.size();
    std::cout << " connected components. CC ran in " << ioctmib_results.time_ccs;
    std::cout << std::endl;

    // Get correct number edges in prescribed OCT decomp, if provided
    if (input_oct_set.size() > 0) {
        Graph temp_graph = g.subgraph(input_oct_set);
        ioctmib_results.num_oct_edges_given = temp_graph.get_num_edges();
    }


    if (vector_of_ccs.size() > 1) {
        OrderedVertexSet oct_set_cc, left_set_cc, right_set_cc;

        // run octmib on each CC
        for (size_t which_cc = 0; which_cc < vector_of_ccs.size(); which_cc++) {

            // Time this CC
            begint = std::clock();

            std::vector<size_t> vertex_subset = vector_of_ccs[which_cc];
            ioctmib_results.relabeling_mode = false;

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
                ioctmib_results.isolates ++;
                continue;
            }

            // Isolated edges are MIBs
            if (vertex_subset.size() == 2) {
                BicliqueLite temp((std::vector<size_t>){vertex_subset.front()},
                                  (std::vector<size_t>){vertex_subset.back()});
                ioctmib_results.push_back_bipartite(temp);
                ioctmib_results.size_left ++;
                ioctmib_results.size_right ++;
                continue;
            }

            Graph g_cc = g.subgraph(vertex_subset);

            // Restrict OCT decomposition to this subgraph
            if (input_oct_set.size() > 0) {
                oct_set_cc = input_oct_set.set_intersection(vertex_subset);
                left_set_cc = input_left_set.set_intersection(vertex_subset);
                right_set_cc = input_right_set.set_intersection(vertex_subset);
            }

            // relabel oct decomposition using subgraph labels
            oct_set_cc = convert_node_labels_OVS(oct_set_cc, reverse_ordering);
            left_set_cc = convert_node_labels_OVS(left_set_cc, reverse_ordering);
            right_set_cc = convert_node_labels_OVS(right_set_cc, reverse_ordering);

            ioctmib_results.turn_on_relabeling_mode(vertex_subset);
            ioctmib_cc(ioctmib_results, g_cc, oct_set_cc, left_set_cc, right_set_cc);

            endt = std::clock();
            std::cout << "# this CC ran in ";
            std::cout << double(endt - begint) / CLOCKS_PER_SEC;
            std::cout << "\n#\n# " << std::endl;

        }

    }
    else {
        octmib_cc(ioctmib_results, g, input_oct_set, input_left_set, input_right_set);
    }

    ioctmib_results.total_num_mibs += ioctmib_results.bipartite_num_mibs;
    ioctmib_results.close_results();

}
