/**
* This implements octmica, our improved version of OCTMIB
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*
* NOTE:
*   This code assumes no node is isolated.
*/

#include "OCTMICA.h"
#include "SimpleCCs.h"
#include "../graph/OrderedVector.h"

void octmica_cc(OutputOptions & octmica_results,
        const Graph & g,
        const OrderedVertexSet input_oct_set,
        const OrderedVertexSet input_left_set,
        const OrderedVertexSet input_right_set) {
	//std::cout << "in octmica_cc" << std::endl;
	std::set<BicliqueLite> hash_set;
	std::stack<BicliqueLite> stack;
	// Step (2) - generate Bicliques based on the MIS from the neighborhoods of the OCT set.
	OrderedVertexSet left_right = input_left_set.set_union(input_right_set);

	OctGraph og(g, input_oct_set, input_left_set, input_right_set);
	const OrderedVertexSet & left_partition = og.get_left();
	const OrderedVertexSet & oct_set = og.get_oct();
	const OrderedVertexSet & right_partition = og.get_right();
	const OrderedVector left_partition_OV(left_partition.get_vertices());
    const OrderedVector oct_set_OV(oct_set.get_vertices());
    const OrderedVector right_partition_OV(right_partition.get_vertices());

    OrderedVector non_oct_vertices(left_partition_OV.set_union(right_partition_OV));

	OutputOptions temp;
	Graph g_minus_oct = og.subgraph(non_oct_vertices);
        std::vector<size_t> bipartite_left, bipartite_right;

	for (size_t idx=0; idx< non_oct_vertices.size(); idx++){
            if (left_partition.has_vertex(non_oct_vertices[idx])) {
                bipartite_left.push_back(idx);
            }
            else bipartite_right.push_back(idx);
        }
	maximal_crossing_bicliques_bipartite(temp,
                                             g_minus_oct,
                                             bipartite_left,
                                             bipartite_right);

	std::set<BicliqueLite> C;
	//std::cout << "After call to MCBB, found: " << temp.mibs_computed.size() << std::endl;
	for (auto b_itr = temp.mibs_computed.begin(); b_itr != temp.mibs_computed.end(); b_itr++) {
		BicliqueLite b = *b_itr;
		std::vector<size_t> left, right, temp;
		left = b.get_left();
		right = g.get_neighborhood_intersection(left, false);
		left = g.get_neighborhood_intersection(right, false);
		C.insert(BicliqueLite(left,right));
	}


	std::set<BicliqueLite> C0;
	for (auto &v : oct_set) {
		std::vector<size_t> left, right, temp;
		left = g.get_neighbors_vector(v);
		//std::cout << "Neighbors vector for " << v << " has size " << left.size() << " and elements: ";
		//for (auto itr = left.begin(); itr != left.end(); itr++) {
		//	std::cout << *itr << " ";
		//}
		//std::cout << std::endl;
		right = g.get_neighborhood_intersection(left, false);
		//if right does not have the original vertex, add it in (should not be necessary)
		if (find(right.begin(), right.end(), v) == left.end()) {
			right.push_back(v);
		}
		auto it = C0.insert(BicliqueLite(left, right));
		//std::cout << "before putting in biclique object, left and right are:" << std::endl;
		//for (auto itr = left.begin(); itr != left.end(); itr++) {
		//	std::cout << *itr << " ";
		//}
		//std::cout << std::endl;
		//for (auto itr = right.begin(); itr != right.end(); itr++) {
		//	std::cout << *itr << " ";
		//}
		//std::cout << std::endl;
		//std::cout << "For starting vertex: " << v << " found max'l biclique (" << g.is_biclique(*(it.first)) << "):" << std::endl;
		//std::cout << g.biclique_string(*(it.first)) << std::endl;
	}
	mica_initialized(octmica_results, g, C0, C);
}

/**
 * Wrapper for our OCTMIB algorithm together that first separates out
 * connected components, runs our algorithm on each CC, and aggregates
 * the results.
 */
void octmica(OutputOptions & octmica_results,
             const Graph & g,
             OrderedVertexSet input_oct_set,
             OrderedVertexSet input_left_set,
             OrderedVertexSet input_right_set) {

    //std::cout << "in octmica" << std::endl;
    octmica_results.set_base_graph(g);
    octmica_results.n = g.get_num_vertices();
    octmica_results.m = g.get_num_edges();

    // Determine connected components
    clock_t begint = std::clock();
    std::vector<std::vector<size_t>> vector_of_ccs = simpleccs(g);
    clock_t endt = std::clock();
    octmica_results.time_ccs += double(endt - begint) / CLOCKS_PER_SEC;
    octmica_results.num_connected_components = vector_of_ccs.size();

    std::cout << "# Graph has " << vector_of_ccs.size();
    std::cout << " connected components. CC ran in " << octmica_results.time_ccs;
    std::cout << std::endl;

    // Get correct number edges in prescribed OCT decomp, if provided
    if (input_oct_set.size() > 0) {
        Graph temp_graph = g.subgraph(input_oct_set);
        octmica_results.num_oct_edges_given = temp_graph.get_num_edges();
    }


    if (vector_of_ccs.size() > 1) {
        OrderedVertexSet oct_set_cc, left_set_cc, right_set_cc;

        // run octmib on each CC
        for (size_t which_cc = 0; which_cc < vector_of_ccs.size(); which_cc++) {

            // Time this CC
            begint = std::clock();

            std::vector<size_t> vertex_subset = vector_of_ccs[which_cc];
            octmica_results.relabeling_mode = false;

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
                octmica_results.isolates ++;
                continue;
            }

            // Isolated edges are MIBs
            if (vertex_subset.size() == 2) {
                BicliqueLite temp((std::vector<size_t>){vertex_subset.front()},
                                  (std::vector<size_t>){vertex_subset.back()});
                octmica_results.push_back_bipartite(temp);
                octmica_results.size_left ++;
                octmica_results.size_right ++;
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

            octmica_results.turn_on_relabeling_mode(vertex_subset);
            octmica_cc(octmica_results, g_cc, oct_set_cc, left_set_cc, right_set_cc);

            endt = std::clock();
            std::cout << "# this CC ran in ";
            std::cout << double(endt - begint) / CLOCKS_PER_SEC;
            std::cout << "\n#\n# " << std::endl;

        }

    }
    else {
        octmica_cc(octmica_results, g, input_oct_set, input_left_set, input_right_set);
    }

    octmica_results.total_num_mibs += octmica_results.bipartite_num_mibs;
    octmica_results.close_results();

}
