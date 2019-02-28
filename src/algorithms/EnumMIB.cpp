/**
* This implements LexMIB, our modified version of the algorithm of Dias et al.
* for enumerating all maximal induced bicliques in a graph in lexicographical
* order. See our associated paper for an explanation of how our modified
* version differs from the original algorithm of Dias et al. presented here:
*
*   V. Dias, C. De Figueiredo, and J. Szwarcfiter, Generating bicliques of a
*   graph in lexicographic order, Theoretical Computer Science, 337 (2005),
*   pp. 240–248.
*
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*
* NOTE:
*   This code assumes no node is isolated.
*/

#include "EnumMIB.h"
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
BicliqueLite nonlex_least_biclique(const Graph & g,
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
void check_for_mib_nonlex(const Graph& g,
                   OrderedVector& x_iter,
                   OrderedVector& y_iter,
                   size_t idx,
                   BicliqueArchiveNonLex& mib_archive
                  ) {

    const OrderedVertexSet &neighb_of_idx = g.get_neighbors(idx);

    // Check if
    // [1] intersect(X_j, N_j) not empty
    // [2] intersect(Y_j, N_j^c) not empty
    // [3] Y_j empty


        // Compute altered versions of x and y -> x' and y'
        OrderedVector x_edited = x_iter;
        x_edited.vector_minus_neighborhood(g, idx);
        x_edited.bubble_sort(idx);

        OrderedVector y_edited = y_iter;
        y_edited.vector_intersect_neighborhood(g, idx);

        std::vector<size_t> lookup_table(g.get_num_vertices(), 0);
        for (size_t u: x_edited) lookup_table[u] = 1;
        for (size_t u: y_edited) lookup_table[u] = 1;

        


	auto this_mib = nonlex_least_biclique(g, x_edited, y_edited);

	// If mib!=0 and not in the archive, add to archive and heap
	if ((this_mib.size() != 0) &&
	(!mib_archive.has_biclique(this_mib)) ) {
		mib_archive.push(this_mib);
	}
	   


}


/**
 * Runs LexMIB assuming the input graph is connected.
 * NonLexMIBResults object is for tracking statistics related to the algorithm's
 * performance.
 */
void enummib_cc(NonLexMIBResults & nonlexmibresults,
                     const Graph & g) {

    // Set up data structures:
    // mib_archive -- includes hashtable and queue.
    //      - hashtable of strings for bicliques already in heap
    //      - queue  (heap) for quick access to bicliques
    BicliqueArchiveNonLex mib_archive;
    size_t num_vertices = g.get_num_vertices();

    // Find least biclique in G.
    std::vector<size_t> initial_set = {0};  // (This code assumes no node is isolated, so vertex 0 is guaranteed to be in.)
    std::vector<size_t> empty_set;

    BicliqueLite least_biclique = nonlex_least_biclique(g, initial_set, empty_set);

    mib_archive.push(least_biclique);

    while (mib_archive.heap_size() > 0) {

        // Find least biclique in Q, call it B, add to output
        auto current_least_bic = mib_archive.top();
        mib_archive.pop();

        nonlexmibresults.push_back(current_least_bic);

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

                check_for_mib_nonlex(g, *set1, *set2, idx, mib_archive);

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
void enummib(NonLexMIBResults & nonlexmibresults, const Graph & g) {

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
                nonlexmibresults.relabeling_mode = false;
                BicliqueLite temp((std::vector<size_t>){vertex_subset.front()},
                             (std::vector<size_t>){vertex_subset.back()});
                nonlexmibresults.push_back(temp);
                continue;
            }

            Graph g_cc = g.subgraph(vertex_subset);

            // Call main LexMIB function
            nonlexmibresults.turn_on_relabeling_mode(vertex_subset);
            enummib_cc(nonlexmibresults, g_cc);

        }
    }
    else {
    	enummib_cc(nonlexmibresults, g);
    }

    nonlexmibresults.close_results();

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
std::vector<BicliqueLite> enummib(const Graph & g) {
    NonLexMIBResults nonlexmibresults;
    enummib(nonlexmibresults, g);
    return nonlexmibresults.mibs_computed;
}
