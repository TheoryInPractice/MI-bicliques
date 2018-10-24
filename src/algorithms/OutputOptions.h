/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_OUTPUTOPTIONS_H
#define BICLIQUES_OUTPUTOPTIONS_H

#include "../graph/BicliqueLite.h"
#include "../graph/OrderedVertexSet.h"

#include "stddef.h"  // apparently required for size_t
#include <memory>  // for std::shared_ptr
#include <fstream>
#include <vector>

#include "../graph/Graph.h"


/**
 * This object used for both MCBB and OCTMIB
 */
class OutputOptions {

private:
    //
public:

    std::vector<size_t> relabeling_vector;
    std::ofstream output_file;

    std::shared_ptr<std::unordered_map<std::string, bool>> external_duplicates_table_ptr;
    std::shared_ptr<const Graph> external_graph_ptr;
    OrderedVertexSet external_oct_set;

    std::string print_mibs_dest = std::string("");
    bool print_mode = false;
    bool count_only_mode = false;
    bool relabeling_mode = false;
    bool check_maximality_mode = false;

    long mib_limit_value = -2;
    bool mib_limit_flag = false;


    double time_oct_decomp = 0.0;
    double time_bipartite_mcb = 0.0;
    double time_bipartite_mcb_check = 0.0;
    double time_oct_MIS = 0.0;
    double time_ccs = 0.0;

    double time_blueprint_init = 0.0;
    double time_iter_mis = 0.0;
    double time_mcbs = 0.0;
    double time_mcb_checking = 0.0;
    double time_search_tree_expand = 0.0;

    size_t total_num_mibs = 0;
    size_t bipartite_num_mibs = 0;

    size_t num_oct_edges_given = 0;
    size_t num_oct_vertices_given = 0;
    size_t size_left_given = 0;
    size_t size_right_given = 0;

    size_t num_oct_mis = 0;
    size_t num_oct_mis_completed = 0;
    size_t num_oct_iter_mis = 0;
    size_t num_oct_iter_mis_completed = 0;

    size_t num_oct_vertices = 0;
    size_t num_oct_edges = 0;
    size_t size_left = 0;
    size_t size_right = 0;
    size_t n = 0;
    size_t m = 0;

    size_t isolates = 0;

    size_t num_connected_components = 0;


    std::vector<BicliqueLite> mibs_computed = {};

    void set_base_graph(const Graph & graph){
        external_graph_ptr = std::make_shared<const Graph>(graph);
    }


    void turn_on_print_mode(const std::string & ofile) {
        print_mibs_dest = ofile;
        print_mode = true;
        output_file.open(print_mibs_dest, std::ios::app);
    }

    template <typename T>
    inline void turn_on_relabeling_mode(const T & relabeling) {
        turn_on_relabeling_mode(relabeling.get_vertices());
    }

    void turn_on_relabeling_mode(const std::vector<size_t> & relabeling) {
        if (!relabeling_mode) {

            relabeling_mode = true;
            relabeling_vector = relabeling;
            return;
        }
        else {

            std::vector<size_t> temp;
            for (auto label: relabeling){
                temp.push_back(relabeling_vector[label]);
            }
            relabeling_vector = move(temp);

        }
    }

    void turn_off_relabeling_mode() {
        relabeling_mode = false;
        relabeling_vector.clear();
    }

    void turn_off_max_check_mode() {
        check_maximality_mode = false;
        external_oct_set = OrderedVertexSet();
    }

    void turn_on_max_check_mode(const std::unordered_map<std::string, bool> & duplicates_table,
                                const OrderedVertexSet & external_oct) {

        check_maximality_mode = true;
        external_duplicates_table_ptr = std::make_shared<std::unordered_map<std::string, bool>> (duplicates_table);

        if (relabeling_mode){
            std::vector<size_t> temp;
            for (auto label: external_oct){
                temp.push_back(relabeling_vector[label]);
            }
            external_oct_set = OrderedVertexSet(temp);
        }
        else {
            external_oct_set = external_oct;
        }

    }

    void close_results() {
        if (print_mode) output_file.close();
    }

    inline void record_biclique(BicliqueLite this_mib){

        // check for recording modes
        if (relabeling_mode) convert_node_labels_biclique(this_mib, relabeling_vector);

        if (print_mode) output_file << this_mib.to_string() << std::endl;
        else mibs_computed.push_back(this_mib);

    }

    inline void push_back_bipartite(const BicliqueLite & this_mib) {
        // increment number of mibs regardless of mode
        bipartite_num_mibs++;
        if (!count_only_mode) record_biclique(this_mib);
    }

    inline void push_back(const BicliqueLite & this_mib) {
        // increment number of mibs regardless of mode
        total_num_mibs++;
        if (!count_only_mode) record_biclique(this_mib);
    }

    void push_back_with_maximality_check(const BicliqueLite & this_mib) {

        if (!check_maximality_mode) push_back_bipartite(this_mib);

        else {

            bool maximality_indicator = true;
            BicliqueLite temp_biclique = this_mib;

            if (relabeling_mode) {
                convert_node_labels_biclique(temp_biclique, relabeling_vector);
            }

            for (auto oct_node: external_oct_set) {

                if (external_graph_ptr->can_be_added_to_biclique(oct_node, temp_biclique)) {
                    maximality_indicator = false;
                    return;
                }
            }

            if (maximality_indicator) {
                std::string this_biclique_string = temp_biclique.to_string();

                if (external_duplicates_table_ptr->find(this_biclique_string) == external_duplicates_table_ptr->end()) {

                    (*external_duplicates_table_ptr)[this_biclique_string] = true;

                    bipartite_num_mibs++;

                    if (!count_only_mode) {
                        if (print_mode) output_file << this_biclique_string << std::endl;
                        else mibs_computed.push_back(temp_biclique);
                    }
                }
            }
        }
    }

};


#endif //BICLIQUES_OUTPUTOPTIONS_H
