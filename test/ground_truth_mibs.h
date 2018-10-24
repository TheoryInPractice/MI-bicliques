/**
 * Contains all the ground truth MIBs for our test datasets.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 */


#ifndef BICLIQUES_GROUNDTRUTHMIBS_H
#define BICLIQUES_GROUNDTRUTHMIBS_H

#include <sstream>
#include <string>
#include "../src/graph/OrderedVertexSet.h"
#include "../src/graph/BicliqueLite.h"


class ground_truth_mibs{

public:

    std::vector<std::vector<size_t>> mibs_true_raw(std::string fname) {

        // Create a filestream for input file
        std::ifstream infile(fname);

        // Start reading from input file
        std::string line_string;
        std::vector<std::vector<size_t>> mibs_list;

        while(std::getline(infile, line_string)) {

            // If the row is non empty, read it & make into an OrderedVertexSet
            std::stringstream line(line_string);

            size_t vertex = 0;
            std::vector<size_t> dummy_vector;

            while (line >> vertex) {
                dummy_vector.push_back( vertex );
            }

            std::sort(dummy_vector.begin(), dummy_vector.end());
            // OrderedVertexSet temp_set(dummy_vector);

            mibs_list.push_back(dummy_vector);
        }

        infile.close();

        return mibs_list;
    }

};


#endif //BICLIQUES_GROUNDTRUTHMIBS_H
