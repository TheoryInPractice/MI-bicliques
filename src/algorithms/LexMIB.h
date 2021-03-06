/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/

#ifndef BICLIQUES_LEXMIB_H
#define BICLIQUES_LEXMIB_H

#include <fstream>
#include <queue>
#include <vector>
#include "../graph/BicliqueLite.h"
#include "../graph/Graph.h"
#include "../graph/OrderedVector.h"



/**
* This struct enables the algorithm to track a number of statistics
* related to the performance of the algorithm like runtime, and the bicliques
* found.
*/
struct LexMIBResults {

    std::string print_mibs_dest = std::string("");
    bool print_mode = false;
    bool count_only_mode = false;
    bool relabeling_mode = false;

    double time_ccs = 0.0;

    size_t total_num_mibs = 0;

    size_t n = 0;
    size_t m = 0;

    std::vector<BicliqueLite> mibs_computed = {};
    std::vector<size_t> relabeling_vector;
    std::ofstream output_file;

    void turn_on_print_mode(std::string ofile) {
        print_mibs_dest = ofile;
        print_mode = true;
        output_file.open(print_mibs_dest, std::ios::app);
    }

    void turn_on_relabeling_mode(std::vector<size_t> relabeling) {
        relabeling_mode = true;
        relabeling_vector = move(relabeling);
    }

    void close_results() {
        if (print_mode) output_file.close();
    }

    void push_back(BicliqueLite this_mib) {
        // increment number of mibs regardless of mode
        total_num_mibs++;

        // check for recording modes
        if (!count_only_mode) {
            if (relabeling_mode) {
                convert_node_labels_biclique(this_mib, relabeling_vector);
            }
            if (print_mode) {
                output_file << this_mib.to_string_persistent() << std::endl;
            }
            else {
                mibs_computed.push_back(this_mib);
            }
        }
    }

};


// Defining ordering operator for the sorting aspect of BicliqueArchive
class BicliqueLexCompare
{
public:
    inline bool operator() (BicliqueLite & mib1, BicliqueLite & mib2) {
        return (mib1.get_all_vertices_persistent() > mib2.get_all_vertices_persistent()) ;
    }
};

/**
 * This class combines a priority queue (min heap) for quick access to the
 * lex least biclique stored, with a hashtable for efficiently checking whether
 * a biclique has been stored in the heap.
 * The heap stores BicliqueLite objects, while the hashtable stores strings.
 */
class BicliqueArchive {
    private:

        // NOTE:
        // This sorting operation was mistakenly used in some experiments. We
        // believe it shouldn't affect runtime, but does affect the ordering.
        // std::priority_queue<BicliqueLite, std::vector<BicliqueLite>, std::less<BicliqueLite> > mib_heap;

        // This sorting operation is the correct one to use.
        std::priority_queue<BicliqueLite, std::vector<BicliqueLite>, BicliqueLexCompare > mib_heap;

        std::unordered_map<std::string,bool> map;

    public:
        inline size_t map_size() { return map.size(); }
        inline size_t heap_size() { return mib_heap.size(); }

        inline bool has_biclique(BicliqueLite & mib) {
            auto this_string = mib.to_string_persistent();
            return  (map.find(this_string) != map.end());
        }

        inline void push(BicliqueLite & mib) {
            map[mib.to_string_persistent()] = true;
            mib_heap.push(mib);
        }

        inline BicliqueLite top() { return mib_heap.top(); }
        inline void pop() { mib_heap.pop(); }

};


BicliqueLite lex_least_biclique(const Graph & g,
                                const OrderedVector & set_x,
                                const OrderedVector & set_y);


void check_for_mib(const Graph & g,
                   OrderedVector & x_iter,
                   OrderedVector & y_iter,
                   size_t idx,
                   BicliqueArchive & mib_archive
                  );


/**
* Given a graph and LexMIBResults object, this updates lexmibresults to contain
* a number of statistics of the graph and the performance of the algorithm.
*/
void lexmib(LexMIBResults & lexmibresults, const Graph & g);

/**
* Given a graph this simply computes and outputs a vector of bicliques found.
*/
std::vector<BicliqueLite> lexmib(const Graph & g);

/**
* Intended for internal use; this function assumes the input graph is connected.
*/
void lexmib_cc(LexMIBResults & lexmibresults,
                     const Graph & g);


#endif //BICLIQUES_LEXMIB_H
