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
 * Implementing algorithm "Least Biclique" from Dias et al.
 *
 * Given a graph with an ordering on its vertices, and two input
 * sets x and y, with x non-empty, output the lexicographically
 * least biclique B(X,Y) in G such that x in X and y in Y, if such
 * a B exists; if there is no such B, output empty-Biclique.
 *
 * Warning: the inputs x and y must be a biclique, i.e. x and y
 * are each independent sets, and x and y are completely connected.
 *
 * If one of the input sets is empty, handle that case separately:
 * check if the non-empty set is completely connected to any nodes.
 * If not, then no B exists, so output the empty set.
 * Otherwise, a non-empty set is found, and the the rest of the
 * algorithm can proceeds.
 *
 */
BicliqueLite lex_least_biclique(const Graph & g,
                                const OrderedVector & set_x_input,
                                const OrderedVector & set_y_input) {

    OrderedVector set_x;
    OrderedVector set_y;
    // Swap sets so X is non-empty, if possible.
    if (set_x_input.size() == 0 && set_y_input.size()!= 0){
        set_x = set_y_input;
        set_y = set_x_input;
    }
    else {
        set_x = set_x_input;
        set_y = set_y_input;
    }

    size_t n = g.get_num_vertices();
    std::vector<size_t> combined_set(n);
    for (auto u: set_x) combined_set[u] = 1;
    for (auto u: set_y) combined_set[u] = 1;


    // Handle the case that Y is empty: check if X is completely
    // connected to any nodes, and add those to Y.
    if (set_y.size() == 0) {
        // Compute intersection of neighborshoods of each v in set_x
        // Note: this is the same as F(X) in Dias et al paper, the "focus" of X.
        OrderedVector set_x_neighb_intersection(g.get_neighborhood_intersection(set_x));

        // If neighborhood intersection of set_x is empty, return empty
        if (set_x_neighb_intersection.size() == 0) {

            return BicliqueLite();
        }

        // Otherwise, update set_x and set_y to what we'll actually use.
        for (size_t current_vertex = 0; current_vertex < n; current_vertex++) {
            // skip vertices in input biclique
            if (combined_set[current_vertex] == 1) continue;

            // If v indepedent from X
            // and v has at least one neighbor in F(X):
            if (g.is_completely_independent_from(current_vertex, set_x) && !g.is_completely_independent_from(current_vertex, set_x_neighb_intersection)) {
                //Update set x, its neighborhood set, and lookup table
                set_x.bubble_sort(current_vertex);
                set_x_neighb_intersection.vector_intersect_neighborhood(g, current_vertex);
                combined_set[current_vertex] = 1;
            }
            if (g.is_completely_connect_to(current_vertex, set_x)) {
                set_y.bubble_sort(current_vertex);
                combined_set[current_vertex] = 1;
                break;
            }
        }

    } // end IF set_y is empty

    // Proceed with update set_x and set_y.
    for (size_t current_vertex = 0; current_vertex < n; current_vertex++) {

        if (combined_set[current_vertex] == 1) continue;

        else if (g.is_completely_independent_from(current_vertex, set_x) && g.is_completely_connect_to(current_vertex, set_y)) {
            set_x.bubble_sort(current_vertex);
        }

        if (g.is_completely_independent_from(current_vertex, set_y) && g.is_completely_connect_to(current_vertex, set_x)) {
            set_y.bubble_sort(current_vertex);
        }

    }

    return BicliqueLite(set_x, set_y);
}


/**
 * WARNING: function not supposed to be called on vertices idx contained
 * in either x_iter or y_iter.
 */
void check_for_mib(const Graph& g,
                   OrderedVector& x_iter,
                   OrderedVector& y_iter,
                   size_t idx,
                   BicliqueArchive& mib_archive
                  ) {

    const OrderedVertexSet &neighb_of_idx = g.get_neighbors(idx);

    // Check if
    // [1] intersect(X_j, N_j) not empty
    // [2] intersect(Y_j, N_j^c) not empty
    // [3] Y_j empty
    if (!g.is_completely_independent_from(idx, x_iter)
        || !g.is_completely_connect_to(idx, y_iter)
        || y_iter.size() == 0) {

        // Compute altered versions of x and y -> x' and y'
        OrderedVector x_edited = x_iter;
        x_edited.vector_minus_neighborhood(g, idx);
        x_edited.bubble_sort(idx);

        OrderedVector y_edited = y_iter;
        y_edited.vector_intersect_neighborhood(g, idx);

        std::vector<size_t> lookup_table(g.get_num_vertices(), 0);
        for (size_t u: x_edited) lookup_table[u] = 1;
        for (size_t u: y_edited) lookup_table[u] = 1;

        // check if B' = (x', y') extends to a biclique for some ell not in (x,y)
        bool no_extension_exists = true;
        for (size_t ell=0; ell<=idx; ell++) {

            // Skip ell in B(x,y) intersect [ 1 , ... , idx ]
            if (lookup_table[ell] == 1) continue;

            // check if ell can be added to either side of biclique
            if (g.is_completely_connect_to(ell, x_edited)
                && g.is_completely_independent_from(ell, y_edited)) {
                no_extension_exists = false;

                break;
            }

            // Handle y_edited == 0 as a special case
            if (g.is_completely_connect_to(ell, y_edited) // returns true for empty y_edited
                && g.is_completely_independent_from(ell, x_edited)) {

                if (y_edited.size() !=0 ) {
                    no_extension_exists = false;
                    break;
                }
                else {

                    OrderedVector x_edited_ell = x_edited;
                    x_edited_ell.bubble_sort(ell);

                    auto temp = g.get_neighborhood_intersection(x_edited_ell, true);
                    if (temp.size() > 0) {
                        no_extension_exists = false;
                        ell = idx + 1;
                        break;
                    }
                }
            }
        }// END for loop over ell not in (x,y)

        if (no_extension_exists) {

            auto this_mib = lex_least_biclique(g, x_edited, y_edited);

            // If mib!=0 and not in the archive, add to archive and heap
            if ((this_mib.size() != 0) &&
            (!mib_archive.has_biclique(this_mib)) ) {
                mib_archive.push(this_mib);
            }
            else {  // if lex_least_biclique returns empty-set
                for (size_t v: neighb_of_idx) {

                    OrderedVector temp_set = x_edited;
                    temp_set.vector_intersect_neighborhood(g, v);

                    BicliqueLite temp_mib = lex_least_biclique(g,
                                                       temp_set,
                                                       OrderedVector((std::vector<size_t>) {v}));
                    if ((temp_mib.size() != 0) &&
                    (!mib_archive.has_biclique(temp_mib)) ) {

                        mib_archive.push(temp_mib);
                    }
                }
            }
        } // End "if no extension exists"
    }
}


/**
 * Runs LexMIB assuming the input graph is connected.
 * LexMIBResults object is for tracking statistics related to the algorithm's
 * performance.
 */
void lexmib_cc(LexMIBResults & lexmibresults,
                     const Graph & g) {

    // Set up data structures:
    // mib_archive -- includes hashtable and priority_queue.
    //      - hashtable of strings for bicliques already in heap
    //      - priority_queue  (heap) for quick access to least biclique
    BicliqueArchive mib_archive;
    size_t num_vertices = g.get_num_vertices();

    // Find least biclique in G.
    std::vector<size_t> initial_set = {0};  // (This code assumes no node is isolated, so vertex 0 is guaranteed to be in.)
    std::vector<size_t> empty_set;

    BicliqueLite least_biclique = lex_least_biclique(g, initial_set, empty_set);

    mib_archive.push(least_biclique);

    while (mib_archive.heap_size() > 0) {

        // Find least biclique in Q, call it B, add to output
        auto current_least_bic = mib_archive.top();
        mib_archive.pop();

        lexmibresults.push_back(current_least_bic);

        // Prep lookup table (note: this could be replaced with a hashtable,
        // but this would only be more efficient if the number of vertices
        // in the graph were rather large, in which case the algorithm likely
        // won't terminate within days.)
        const std::vector<size_t> & set_x = current_least_bic.get_left();
        const std::vector<size_t> & set_y = current_least_bic.get_right();
        std::vector<size_t> lookup_table(num_vertices);
        for (size_t u: set_x) lookup_table[u] = 1;
        for (size_t u: set_y) lookup_table[u] = 2;

        OrderedVector set_x_iter, set_y_iter;

        // Loop over vertices not in B
        for (size_t idx=0; idx < num_vertices; idx++) {

            // If idx is in B = (set_x, set_y), add that vertex to the
            // iterative sets, then skip
            bool skip_this = false;
            if (lookup_table[idx] == 1) {
                set_x_iter.push_back(idx);
                skip_this = true;
            }
            if (lookup_table[idx] == 2) {
                set_y_iter.push_back(idx);
                skip_this = true;
            }
            if (skip_this) continue;

            // try (x,y) then try (y,x)
            std::vector<OrderedVector*> pair = {&set_x_iter, &set_y_iter};
            for (auto x : {0,1}) {
                OrderedVector * set1 = pair[x];
                OrderedVector * set2 = pair[1-x];

                check_for_mib(g, *set1, *set2, idx, mib_archive);

            } // Trying (x,y) and (y,x)

        } // end FOR over vertices of G - B

    } // end WHILE mib_heap not empty

    // return res;
}


/**
 * Wrapper for MIB enumeration algorithm LexMIB that first separates out
 * connected components, runs the algorithm on each CC, and aggregates
 * the results.
 */
void lexmib(LexMIBResults & lexmibresults, const Graph & g) {

    // Determine connected components
    auto vector_of_ccs = simpleccs(g);

    if (vector_of_ccs.size() > 1) {
        // run algorithm on each CC
        for (size_t which_cc = 0; which_cc < vector_of_ccs.size(); which_cc++) {

            const std::vector<size_t> vertex_subset = vector_of_ccs[which_cc];

            // Skip isolated vertices and empty sets
            if (vertex_subset.size() <= 1) continue;

            // Isolated edges are MIBs
            if (vertex_subset.size() == 2) {
                lexmibresults.relabeling_mode = false;
                BicliqueLite temp((std::vector<size_t>){vertex_subset.front()},
                             (std::vector<size_t>){vertex_subset.back()});
                lexmibresults.push_back(temp);
                continue;
            }

            Graph g_cc = g.subgraph(vertex_subset);

            // Call main LexMIB function
            lexmibresults.turn_on_relabeling_mode(vertex_subset);
            lexmib_cc(lexmibresults, g_cc);

        }
    }
    else {
        lexmib_cc(lexmibresults, g);
    }

    lexmibresults.close_results();

}

/**
* Given input graph g, output a vector containing all maximal induced bicliques
* in g, in lexicographic order. Uses LexMIB, our modified version of the
* algorithm of Dias et al.
*
* This is the most general-purpose, easy-to-use function -- we recommend using
* this function unless you need specific information provided by the above
* functions.
*/
std::vector<BicliqueLite> lexmib(const Graph & g) {
    LexMIBResults lexmibresults;
    lexmib(lexmibresults, g);
    return lexmibresults.mibs_computed;
}









/**
 * Edits the given biclique to be maximal with respect to expansion_set1.
 * Then, checks agains expansion_set2 to see if the biclique is maximal wrt set2.
 * Returns true if maximal wrt set2 (and edits the Biclique), false and does not edit otherwise.
 */
bool make_maximal(Graph & g, BicliqueLite & b, OrderedVertexSet expansion_set1, OrderedVertexSet expansion_set2) {
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

BicliqueLite add_to(Graph & g, BicliqueLite & b, bool left, size_t vertex) {
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

	std::unordered_set<BicliqueLite> hash_set;
	std::stack<BicliqueList> stack;
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
		BicliqueLite cur = stack.pop();
		for (auto o_itr = input_oct_set.begin(); o_itr != input_oct_set.end(); o_itr++) {
			//this could be more efficient if we used sets within BicliqueLite
			if (cur.get_left().find(*o_itr) == cur.get_left().end() && cur.get_right().find(*o_itr) == cur.get_right().end()) {
				BicliqueLite m1 = add_to(g, cur, true, *o_itr);
				make_maximal(g, m1, input_oct_set, OrderedVertexSet());
				if (hash_set.insert(m1).second) {
					stack.push(m1);
					ioctmib_results.push_back(m2);
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
