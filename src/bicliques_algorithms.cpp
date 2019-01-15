/**
* @authors Eric Horton, Kyle Kloster, Drew van der Poel
*
* This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
* and is Copyright (C) North Carolina State University, 2018.
* It is licensed under the three-clause BSD license; see LICENSE.
*/


// Includes
#include <unistd.h> // for getopt
#include <ctime> // for timing
#include <csignal> //for printing after timeout/interrupt
#include <iostream>
#include <fstream>
#include "graph/Graph.h"
#include "algorithms/OCTMIB.h"
#include "algorithms/LexMIB.h"
#include "algorithms/MICA.h"
#include "algorithms/SimpleCCs.h"
#include "algorithms/SimpleOCT.h"



inline bool is_file_there(const std::string & fname) {
    std::ifstream f(fname.c_str());
    return f.good();
}


/**
 * Function for parsing a graph's oct decomposition from file.
 * The text file must specify the nodes in the OCT set on one line,
 * and the nodes in one of the two bipartite partitions on another line.
 */
void read_in_oct_decomposition(std::string filename,
                               Graph & input_g,
                               OrderedVertexSet & oct_set,
                               OrderedVertexSet & left_set) {

    // Create a filestream for input file
    std::ifstream infile(filename);

    // Start reading from input file
    std::string vertex_one, vertex_two, line_string;
    std::vector<size_t> dummy_vector;

    size_t row_counter = 1;
    while(std::getline( infile, line_string ))
    {
        // if we've already seen 2 rows, end
        if (row_counter >= 3) break;

        std::stringstream line(line_string);
        // If the row is non empty, read it in, make into an OrderedVertexSet
        if (line >> vertex_one) {
            dummy_vector.clear();

            size_t internal_label = 0;
            if (input_g.get_internal_vertex_label(vertex_one, internal_label)) {
                dummy_vector.push_back( internal_label );
            }
            while(line >> vertex_two) {
                size_t internal_label2 = 0;
                if (input_g.get_internal_vertex_label(vertex_two, internal_label2)) {
                    dummy_vector.push_back( internal_label2 );
                }
            }

            std::sort(dummy_vector.begin(), dummy_vector.end());

            if (row_counter == 1) {
                oct_set = OrderedVertexSet(dummy_vector);
            }
            else if (row_counter == 2) {
                left_set = OrderedVertexSet(dummy_vector);
            }
        }

        row_counter ++;
    }
    infile.close();

}


struct OutputHandler {

    bool successful_termination = false;
    long time_out_value = -1;

    std::string which_algorithm = "";
    std::string input_file_path = "";
    std::string log_file_path = "";

    char alg_char;

    clock_t begin = std::clock(), end = std::clock();
    double elapsed_time = -1.0;

    size_t num_vertices = 0;
    size_t num_edges = 0;
    size_t num_connected_components = 0;

    bool is_it_bipartite = false;

    OutputOptions octmib_results;
    OutputOptions mica_results;
    LexMIBResults lexmib_results;

    void start_timer() { this->begin = std::clock(); }
    void stop_timer() {
        this->end = std::clock();
        this->elapsed_time = double(this->end - this->begin) / CLOCKS_PER_SEC;
    }

    void write_to_log_file() {
        std::ofstream output_file;
        output_file.open(this->log_file_path, std::ios::app);

        switch (this->alg_char) {
            case 'c':
                output_file << "simpleccs " << this->input_file_path;
                output_file << " " << this->num_connected_components;
                output_file << " " << this->num_edges;
                output_file << " " << this->num_vertices;
                output_file << std::endl;
                break;
            case 'b':
                output_file << "simpleoct " << this->input_file_path;
                if (is_it_bipartite) output_file << " bipartite";
                else output_file << " NOT_bipartite";
                output_file << " " << this->num_edges;
                output_file << " " << this->num_vertices;
                output_file << std::endl;
                break;
            case 'o':
                // Ensure we output stats from prescribed decomposition,
                // if it was provided.
                if (octmib_results.size_left_given > 0) {
                    octmib_results.size_left = octmib_results.size_left_given;
                    octmib_results.size_right = octmib_results.size_right_given;
                    octmib_results.num_oct_vertices = octmib_results.num_oct_vertices_given;
                    octmib_results.num_oct_edges = octmib_results.num_oct_edges_given;
                }

                output_file << "OCT-MIB " << this->input_file_path;
                output_file << " " << this->successful_termination;
                output_file << " " << octmib_results.total_num_mibs << std::flush;
                // std::fixed prevents scientific notation
                output_file << std::fixed << " " << this->elapsed_time << std::flush;
                output_file << " " << octmib_results.bipartite_num_mibs << std::flush;

                output_file << " " << octmib_results.time_bipartite_mcb << std::flush;
                output_file << " " << octmib_results.time_iter_mis << std::flush;
                output_file << " " << octmib_results.time_mcbs << std::flush;
                output_file << " " << octmib_results.time_blueprint_init << std::flush;
                output_file << " " << octmib_results.time_mcb_checking << std::flush;
                output_file << " " << octmib_results.time_search_tree_expand << std::flush;

                output_file << " " << octmib_results.time_oct_MIS << std::flush;
                output_file << " " << octmib_results.time_oct_decomp << std::flush;
                output_file << " " << octmib_results.time_ccs << std::flush;
                output_file << " " << octmib_results.num_oct_iter_mis_completed << std::flush;
                output_file << " " << octmib_results.num_oct_iter_mis << std::flush;

                output_file << " " << octmib_results.num_oct_mis_completed << std::flush;
                output_file << " " << octmib_results.num_oct_mis << std::flush;
                output_file << " " << octmib_results.num_oct_edges << std::flush;
                output_file << " " << octmib_results.num_oct_vertices << std::flush;
                output_file << " " << octmib_results.size_left << std::flush;
                output_file << " " << octmib_results.size_right << std::flush;
                output_file << " " << this->num_edges << std::flush;
                output_file << " " << this->num_vertices << std::flush;
                output_file << " " << this->time_out_value << std::flush;
                output_file << " " << octmib_results.mib_limit_value  << std::flush;
                output_file << std::endl;
                break;
            case 'l':
                output_file << "LexMIB " << this->input_file_path;
                output_file << " " << this->successful_termination;
                output_file  << " " << lexmib_results.total_num_mibs;
                // std::fixed prevents scientific notation
                output_file << std::fixed << " " << this->elapsed_time;
                output_file << " " << this->num_edges;
                output_file << " " << this->num_vertices;
                output_file << " " << this->time_out_value;
                output_file << std::endl;
                break;
        }
        output_file.close();
    }
};


// Must be GLOBAL so timeoutHandler can access this
OutputHandler output_tracker;

void timeoutHandler(int signal) {
    std::cout << "Signal received, terminating early" << std::endl;

    output_tracker.stop_timer();

    if (output_tracker.log_file_path!=std::string("")) {
         std::cout << "Saving work." << std::endl;
        output_tracker.write_to_log_file();
    }
    std::cout << "Shutting down." << std::endl;

    exit(signal);
}

/**
 * Call the main OCTMIB.cpp or DiasGeneral.cpp algorithm from commandline
 */
int main(int argc, char ** argv) {

    int error = 1;

    signal(SIGINT, timeoutHandler);
    signal(SIGALRM, timeoutHandler);
    signal(SIGQUIT, timeoutHandler);

    std::string print_results_path;
    std::string oct_file_path;
    bool help_flag = false;
    bool count_only_mode = false;
    int arg_counter;
    while( ( arg_counter = getopt (argc, argv, "a:i:o:l:hp:ct:m:") ) != -1 )
    {
        switch(arg_counter)
        {
            case 'h':
                help_flag = true;
                break;
            case 'a':
                if(optarg) output_tracker.which_algorithm = optarg;
                std::cout << "# Running algorithm " << output_tracker.which_algorithm << std::endl;
                break;
            case 'i':
                if(optarg) output_tracker.input_file_path = optarg;
                std::cout << "# Reading graph from " << output_tracker.input_file_path << std::endl;
                break;
            case 'c':
                std::cout << "# Count only mode." << std::endl;
                count_only_mode = true;
                break;
            case 'p':
                if(optarg) print_results_path = optarg;
                std::cout << "# Printing MIBs to " << print_results_path;
                std::cout << std::endl;
                break;
            case 'o':
                if(optarg) oct_file_path = optarg;
                break;
            case 'l':
                if(optarg) output_tracker.log_file_path = optarg;
                std::cout << "# Writing results to " << output_tracker.log_file_path << std::endl;
                break;
            case 't':
                if(optarg) output_tracker.time_out_value = atol(optarg);
                std::cout <<  "# Running with timeout: " << output_tracker.time_out_value << " seconds.";
                std::cout << std::endl;
                break;
            case 'm':
                if(optarg) {
                    output_tracker.octmib_results.mib_limit_value = atol(optarg);
                    output_tracker.octmib_results.mib_limit_flag = true;
                }
                std::cout <<  "# Running with MIB threshold: " << output_tracker.octmib_results.mib_limit_value << " total MIBs.";
                std::cout << std::endl;
                break;
        }
    }

    if (help_flag) {
        std::cout << "usage: bicliques -a ALGORITHM: l o c b ";
        std::cout << "-i PATH_TO_INPUT_FILE ";
        std::cout << "[-o PATH_TO_OCT_FILE] ";
        std::cout << "[-l PATH_TO_LOG_FILE] [-h] [-c] ";
        std::cout << "[-p PATH_TO_MIBS_FILE] ";
        std::cout << "[-t TIME_OUT_VALUE]" << std::endl;

        std::cout << "Bicliques algorithms suite.\n\n";
        std::cout << "required arguments:\n";
        std::cout << "\tALGORITHM             which algorithm to use: l (LexMIB), ";
        std::cout << "o (OCT-MIB), c (counts # CCs), b (checks if bipartite), m (MICA) \n";
        std::cout << "\tPATH_TO_INPUT_FILE    directory and filename of input graph\n\n";
        std::cout << "optional arguments:\n";
        std::cout << "\t-h                    show this help message and exit\n";
        std::cout << "\t-o                    directory and filename of oct decomposition\n";
        std::cout << "\t-l                    directory and filename where to record results\n";
        std::cout << "\t-p                    directory and filename of where to write out MIBs.\n";
        std::cout << "\t                      Note: this option leaves the internal array of MIBs empty.\n";
        std::cout << "\t-c                    count only mode; MIBs not stored or written to file.\n";
        std::cout << "\t-t                    timeout in seconds to allow program to run.\n";
        std::cout << "\t-m                    terminate if this many MIBs are found.";
        std::cout << std::endl;
        return 1;
    }

    // Check for algorithm
    if (output_tracker.which_algorithm != "l" &&
        output_tracker.which_algorithm != "o" &&
        output_tracker.which_algorithm != "b" &&
        output_tracker.which_algorithm != "c" &&
		output_tracker.which_algorithm != "m") {
        std::cout << "ERROR::BICLIQUES incorrect algorithm specified: ";
        std::cout << output_tracker.which_algorithm << std::endl;
        error = 0;
        return error;
    }
    else if (output_tracker.which_algorithm=="") {
        std::cout << "ERROR::BICLIQUES no algorithm specified.";
        std::cout << std::endl;
        error = 0;
        return error;
    }

    // Check for input graph
    Graph input_g(0);
    if (output_tracker.input_file_path==std::string("")) {
        std::cout << "ERROR::BICLIQUES no input graph provided.";
        std::cout << std::endl;
        error = 0;
        return error;
    }
    else if (!is_file_there(output_tracker.input_file_path)) {
        std::cout << "ERROR::BICLIQUES input file not found.";
        std::cout << std::endl;
        error = 0;
        return error;std::cout << "# Running algorithm " << output_tracker.which_algorithm << std::endl;
    }
    else {
        Graph dummy(output_tracker.input_file_path, Graph::FILE_FORMAT::adjlist);
        input_g = dummy;
    }

    output_tracker.num_vertices = input_g.get_num_vertices();
    output_tracker.num_edges = input_g.get_num_edges();

    // If OCT information is input, read in OCT and left partitions
    OrderedVertexSet oct_set, left_partition;
    if (oct_file_path!=std::string("")) {
        read_in_oct_decomposition(oct_file_path, input_g, oct_set, left_partition);

        if (oct_set.size() > 0) {
            std::cout << "# Successfully read oct decomposition from ";
            std::cout << oct_file_path << std::endl;
            std::cout << "#   size " << oct_set.size() << std::endl;
            output_tracker.octmib_results.num_oct_vertices_given = oct_set.size();
            output_tracker.octmib_results.size_left_given = left_partition.size();
            output_tracker.octmib_results.size_right_given = input_g.get_num_vertices() - oct_set.size() - left_partition.size();

        }
        else {
            std::cout << "# No oct decomposition read from ";
            std::cout << oct_file_path << std::endl;
            std::cout << "#   Using greedy OCT algorithm instead." << std::endl;
        }
    }

    // Run algorithm
    output_tracker.start_timer();

    // add timeout
    if (output_tracker.time_out_value > 0) {
      alarm(output_tracker.time_out_value);
    }
    output_tracker.alg_char = output_tracker.which_algorithm[0];
    switch (output_tracker.alg_char) {
        case 'o':  // run OCTMIB
            std::cout << "# Starting algorithm OCT-MIB" << std::endl;
            if (print_results_path!=std::string("")) {
                output_tracker.octmib_results.turn_on_print_mode(print_results_path);
                output_tracker.octmib_results.count_only_mode = count_only_mode;
            }
            octmib(output_tracker.octmib_results, input_g, oct_set, left_partition);
            break;
        case 'l':  // run LexMIB
            std::cout << "# Starting algorithm LexMIB" << std::endl;
            if (print_results_path!=std::string("")) {
                output_tracker.lexmib_results.turn_on_print_mode(print_results_path);
                output_tracker.lexmib_results.count_only_mode = count_only_mode;
            }
            lexmib(output_tracker.lexmib_results, input_g);
            break;
        case 'c':  // just count connected components
            {
        	    auto temp_ccs_dummy = simpleccs(input_g);
                output_tracker.num_connected_components = temp_ccs_dummy.size();
            }
            break;
        case 'b':  // just test for bipartiteness
            OrderedVertexSet temp_oct, temp_l, temp_r;
            simpleoct(input_g, temp_oct, temp_l, temp_r);
            if (temp_oct.size() == 0) output_tracker.is_it_bipartite = true;
            else output_tracker.is_it_bipartite = false;
            break;
        case 'm': //MICA
        	std::cout << "# Starting algorithm OCT-MIB" << std::endl;
        	if (print_results_path!=std::string("")) {
        		output_tracker.octmib_results.turn_on_print_mode(print_results_path);
        	    output_tracker.octmib_results.count_only_mode = count_only_mode;
        	}
        	mica_cc(output_tracker.mica_results, input_g);
        	break;
    }

    output_tracker.successful_termination = true;
    output_tracker.stop_timer();  // time saved in output_tracker.elapsed_time



    // OUTPUT STATS
    if (output_tracker.log_file_path!=std::string("")) {
        output_tracker.write_to_log_file();
    }

    return error;
}
