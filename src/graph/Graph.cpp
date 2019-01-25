/**
 * Graph is a container class for encoding a graph with functions for
 * efficiently determining connectivity of subsets of vertices.
 * The class stores a graph in both adjacency list and adjacency_matrix
 * formats for efficient neighborhood and individual edge look-ups.
 * Moreover, the class implements efficient checks for whether two specified
 * vertex subsets are completely connected to, or independent from each other.
 * The class enables reading in a graph from a file storing the edge list
 * or adjacency list of a graph.
 *
 * To read in a graph from file.txt, the file's first line must give the number
 * of nodes and the number of edges in the graph. The file must be tab
 * (or space) sepaerated. Vertex labels can numbers or strings.
 *
 * @authors Eric Horton, Kyle Kloster, Drew van der Poel
 *
 * This file is part of MI-bicliques, https://github.com/TheoryInPractice/MI-bicliques,
 * and is Copyright (C) North Carolina State University, 2018.
 * It is licensed under the three-clause BSD license; see LICENSE.
 */


#include "Graph.h"


Graph::Graph(size_t num_vertices)
{

    // Init num vertices
    this->num_vertices = num_vertices;
    this->num_edges = 0;

    // Resize datastructures
    adjacency_matrix.resize(num_vertices * num_vertices);
    adjacency_list.resize(num_vertices);
    adjacency_ordered_vertex_sets.reserve(num_vertices);

}


/*
 * Construct a new graph instance by reading from a file.
 *
 * @param filename File name (or path) to the graph data file.
 * @param format   Format used to define the graph file.
 */
Graph::Graph(std::string filename, FILE_FORMAT format)
{

    // Find format and read from file
    switch (format) {
        case edgelist:
            read_edgelist(filename);
            break;
        case adjlist:
            read_adjlist(filename);
            break;
    }

    // Sort neighborhood lists
    for (auto &n : adjacency_list) std::sort(n.begin(), n.end());

    // Finally, convert sorted neighborhoods to OrderedVertexSets
    adjacency_ordered_vertex_sets.resize(num_vertices);

    for (size_t idx = 0; idx < adjacency_list.size(); idx++) {
        std::vector<size_t> get_new_copy = adjacency_list[idx];
        OrderedVertexSet current_neighborhood(get_new_copy);
        adjacency_ordered_vertex_sets[idx] = current_neighborhood;
    }

}


/*
 * Construct a new graph instance by extracting a subgraph induced
 * by a subset of vertices in an existing graph object.
 * Note that the contents of s must be valid indices of the vector
 * supergraph_adjacency_list.
 *
 * @param s subset of vertices in the graph.
 * @param supergraph_adjacency_list   Adjacency list of this graph.
 */
Graph::Graph(const std::vector<size_t> &s,
             const std::vector<std::vector<size_t>> &supergraph_adjacency_list,
             const std::unordered_map<size_t, std::string> &supergraph_labels) {


    // Create map for easy adjacency checking
    std::unordered_map<size_t,bool> s_map;
    for (auto v: s) s_map[v] = true;

    // Set number of vertices
    this->num_vertices = s.size();
    this->num_edges = 0;

    // Resize datastructures
    adjacency_matrix.resize(num_vertices * num_vertices);
    adjacency_list.resize(num_vertices);
    adjacency_ordered_vertex_sets.reserve(num_vertices);

    // Create node label hashtables so the node labels in the subgraph
    // are consistent with those of the supergraph.
    for (auto v_label_in_s : s) {

        const std::string v_label = supergraph_labels.at(v_label_in_s);

        size_t internal_label = node_labels.size();
        node_labels[internal_label] = v_label;
        reverse_node_labels[v_label] = internal_label;

    }

    // Init all vertices
    for (auto v : s) {

        // Convert to label
        const std::string v_label = supergraph_labels.at(v);

        // Get all neighbors of v
        auto neighbors = supergraph_adjacency_list[v];

        // Get neighbors
        for (auto u : neighbors) {

            // Convert to label; add edge if neighbor also in s
            const std::string u_label = supergraph_labels.at(u);
            if (s_map.find(u) != s_map.end()) {
                add_edge_unsafe(v_label, u_label);
            }

        }

    }

    // Sort neighborhood lists
    for (auto &n : adjacency_list) {
        std::sort(n.begin(), n.end());
        adjacency_ordered_vertex_sets.push_back(OrderedVertexSet(n));
    }

}


/**
 * Creates a graph by extracting a subgraph from this graph, with vertex set
 * specified by an input vector of vertex indices.
 *
 * The subgraph will preserve the same external string labels in this graph
 * so that the external string labels provide a map that identifies the
 * output as a subgraph of this graph.
 *
 * @param vector of vertex indices.
 */
Graph Graph::subgraph(const std::vector<size_t> &s) const {

    Graph subgraph(s, adjacency_list, node_labels);
    return subgraph;

}


/*
 * Read from an adjacency list formatted file and populate both
 * adjacency_matrix and adjacency_list.
 *
 * @param filename File name (or path) to the graph data file.
 */
void Graph::read_adjlist(std::string filename) {

    // Create a filestream for input file
    std::ifstream infile(filename);

    // Read number of vertices and number of edges
    infile >> num_vertices >> num_edges;
    num_edges = 0; // we internally compute number of edges

    // Resize datastructures
    adjacency_matrix.resize(num_vertices * num_vertices);
    adjacency_list.resize(num_vertices);

    // Start reading from input file
    std::string vertex_one, vertex_two, line_string;
    while(std::getline( infile, line_string ))
    {
        std::stringstream line(line_string);
        if (line >> vertex_one) {

            while(line >> vertex_two) {
                add_edge_unsafe(vertex_one, vertex_two);

            }

    	    // Handle isolated vertices
            if (reverse_node_labels.find(vertex_one) == reverse_node_labels.end()) {
                size_t u = node_labels.size();
                node_labels[u] = vertex_one;
                reverse_node_labels[vertex_one] = u;
    	    }

        }
    }
    infile.close();

}


/*
 * Read from an edgelist formatted file and populate both
 * adjacency_matrix and adjacency_list.
 *
 * @param filename File name (or path) to the graph data file.
 */
void Graph::read_edgelist(std::string filename) {

    // Create a filestream for input file
    std::ifstream infile(filename);

    // Read number of vertices and number of edges
    infile >> num_vertices >> num_edges;
    num_edges = 0; // we internally compute number of edges

    // Resize datastructures
    adjacency_matrix.resize(num_vertices * num_vertices);
    adjacency_list.resize(num_vertices);

    // Start reading from input file
    std::string vertex_1, vertex_2;
    while(infile.good())
    {

        if ((infile >> vertex_1) && (infile >> vertex_2)) {

            add_edge_unsafe(vertex_1, vertex_2);

        }

    }
    infile.close();

}


/*
 * Add an edge between two vertices to the graph. This method enables
 * referencing vertices by their external labels, by mapping those labels
 * to the internal vertex labels, and then calling add_edge() defined on
 * internal labels.
 */
bool Graph::add_edge_unsafe(std::string l1, std::string l2) {

    // Compute integer for label l1
    size_t u;
    if (reverse_node_labels.find(l1) != reverse_node_labels.end()) {
        u = reverse_node_labels[l1];
    }
    else {
        u = node_labels.size();
        node_labels[u] = l1;
        reverse_node_labels[l1] = u;
    }

    // Compute integer for label l2
    size_t v;
    if (reverse_node_labels.find(l2) != reverse_node_labels.end()) {
        v = reverse_node_labels[l2];
    }
    else {
        v = node_labels.size();
        node_labels[v] = l2;
        reverse_node_labels[l2] = v;
    }

    return add_edge(u,v);

}


/*
 * Add an edge between two vertices to the graph. This method updates
 * the internal adjacency_matrix and adjacency_list vectors and returns
 * true if an edge was added. Leaves adjacency_list potentially unsorted.
 */
bool Graph::add_edge(size_t u, size_t v) {

    // Calculate symmetric indices
    size_t idx1 = num_vertices * u + v;
    size_t idx2 = num_vertices * v + u;

    // Do nothing if an edge already exists
    if(adjacency_matrix[idx1]) return false;

    // Set matrix
    adjacency_matrix[idx1] = true;
    adjacency_matrix[idx2] = true;

    // Add neighbors
    adjacency_list[u].push_back(v);
    adjacency_list[v].push_back(u);

    num_edges ++;

    return true;

}

/**
 *  Check if graph contains input edge specified by external string labels.
 * Proceed by first converting string labels to graph internal integer
 * labels. If the input string labels are not labels of vertices in the graph,
 * output false. Then call function has_edge(u,v) defined for integer labels.
 * If the edge is present in the adjacency matrix, output true; else false.
 */
bool Graph::has_edge(const std::string & l1, const std::string & l2) {
    // Compute integer for label l1
    size_t u;
    if (reverse_node_labels.find(l1) != reverse_node_labels.end()) {
        u = reverse_node_labels[l1];
    }
    else {
        return false;
    }

    // Compute integer for label l2
    size_t v;
    if (reverse_node_labels.find(l2) != reverse_node_labels.end()) {
        v = reverse_node_labels[l2];
    }
    else {
        return false;
    }

    return has_edge(u,v);

}


/*
 * Checks whether v can be added to the input biclique by checking
 * whether v is completely connected to either side and independent from the
 * other side. Outputs true if v can be added, false otherwise.
 */
bool Graph::can_be_added_to_biclique(const size_t v, const BicliqueLite &b) const {

    const auto & left = b.get_left();
    const auto & right = b.get_right();

    // Use binary search for faster check whether vertex already in biclique.
    if ( std::binary_search(left.begin(), left.end(), v) ) return false;

    if ( std::binary_search(right.begin(), right.end(), v) ) return false;

    if (is_completely_connect_to(v, left)) {
        if (is_completely_independent_from(v, right)) return true;
    }
    else if (is_completely_connect_to(v, right)) {
        if (is_completely_independent_from(v, left)) return true;
    }

    return false;

}


/*
 * Outputs number of vertices in graph.
 */
size_t Graph::get_num_vertices() const {

    return num_vertices;

}

/*
 * Outputs number of edges in graph.
 */
size_t Graph::get_num_edges() const {

    return num_edges;

}



/*
 * Check if vertex has external string label; if yes, set
 * internal_vertex_label to that vertex's internal (size_t) label
 * and return true. Else return false.
 */
bool Graph::get_internal_vertex_label(const std::string & external_vertex_label, size_t& internal_vertex_label) {

    if (reverse_node_labels.find(external_vertex_label) != reverse_node_labels.end()) {
        internal_vertex_label = reverse_node_labels[external_vertex_label];
        return true;
    }

    return false;

}

/*
 * Return external string label of vertex with internal (size_t) label internal_vertex_label
 */
std::string Graph::get_external_vertex_label(size_t internal_vertex_label) {

    return node_labels[internal_vertex_label];

}


/**
 * Given a biclique, concatenate the string labels of each vertex
 * in the biclique, separated by commas.
 *
 * @param  b A Biclique in this graph.
 * @return   string consisting of the string labels of the vertices
 *           in the biclique, ordered, and separated by commas.
 */
std::string Graph::biclique_string(const BicliqueLite &b) const {

    std::stringstream s;

    const std::vector<size_t>& all_vertices = b.get_all_vertices();

    // iterate over every node in the biclique
    for (std::vector<size_t>::const_iterator v = all_vertices.begin(); v != all_vertices.end(); v++) {
	// We know that everything is in the map, so we don't need to check existence first.
	// We are using "find" instead of [] because [] modifies the map if not there and
	// we need this to be const.
        s << node_labels.find(*v)->second << ",";
    }

    return s.str();

}


/**
 * Mostly for debugging purposes; this prints the internal representation of
 * the adjacency list of the graph.
 */
void Graph::print_graph() const {

    std::cout << "\nGraph TEST:" << std::endl;
    std::cout << "" << adjacency_list.size() << std::endl;

    for (size_t idx=0; idx < adjacency_list.size(); idx++){
        std::cout << idx << std::flush;

        for (size_t v: adjacency_list[idx]) {
            std::cout << " " << v << std::flush;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


/**
 * Mostly for debugging purposes; this prints the external representation of
 * the adjacency list of the graph, i.e. using external, string labels.
 */
void Graph::print_graph_with_labels() const {

    std::cout << "\nGraph:" << std::endl;

    std::vector<std::string> dummy_list;
    for (auto iter: this->reverse_node_labels) {
        dummy_list.push_back(iter.first);
    }
    std::sort(dummy_list.begin(), dummy_list.end());

    // Using the hashtables violates this function being const
    // unless we perform these copies here
    auto copy_node_labels = node_labels;
    auto copy_reverse_node_labels = reverse_node_labels;

    for (size_t idx=0; idx < dummy_list.size(); idx++){
        size_t dummy_int = copy_reverse_node_labels[dummy_list[idx]];

        std::cout << "(" << dummy_int << ") " << dummy_list[idx] << ": ";

        for (size_t v: adjacency_list[dummy_int]) {
            std::cout << "(" << v << ")" << copy_node_labels[v] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


/**
 *  For verifying whether a BicliqueLite object is actually a biclique
 */
bool Graph::is_biclique(const BicliqueLite & this_biclique) const {
    auto left = this_biclique.get_left();
    auto right = this_biclique.get_right();

    if (left.size() == 0 || right.size() == 0) return false;

    // first check that no vertex appears multiple times
    std::unordered_map<size_t,size_t> vertex_count;
    for (auto v: left) vertex_count[v] = 0;
    for (auto v: right) vertex_count[v] = 0;

    for (auto v: left) vertex_count[v] ++;
    for (auto v: right) vertex_count[v] ++;

    for (auto & iter: vertex_count) {
        if (iter.second > 1) return false;
    }

    // first check the two sides are completely connected
    for (auto v1: left) {
        for (auto v2: right) {
            if (!has_edge(v1, v2)) return false;
        }
    }

    // now check for independence
    for (auto v1: left) {
        for (auto v2: left) {
            if (v2 >= v1) break;
            if (has_edge(v1, v2)) return false;
        }
    }

    for (auto v1: right) {
        for (auto v2: right) {
            if (v2 >= v1) break;
            if (has_edge(v1, v2)) return false;
        }
    }

    return true;

}


/*
 * Checks whether v is adjacent to every vertex in vertex set s.
 * Outputs false if s is empty -- although this is not technically
 * true, this choice is important for implementation reasons.
 *
 * WARNING: assumes input vector is sorted.
 * Template works with OrderedVertexSet, OrderedVertexSet, and also
 * std::vector<size_t> if sorted in ascending order.
 */
bool Graph::is_completely_connect_to(const size_t v,
                                     const std::vector<size_t> &s) const {
    // If s is empty, return TRUE -- note, this is a design choice;
    // in this case the set s being empty means it can not prevent
    // nodes from being added to the biclique in our larger algorithm.

    if ( s.size() == 0 ) return true;

    const std::vector<size_t> & v_neighborhood = get_neighbors_vector(v);

    // If v's neighborhood too small, can't contain all of s
    if ( v_neighborhood.size() < s.size() ) return false;

    // if range of v's neighborhood does not contain range of s, return false
    if ((s.front() < v_neighborhood.front()) || (s.back() > v_neighborhood.back())) {
        return false;
    }

    // for each j in s check if j is adjacent to v
    for (auto j : s) if (!has_edge(v,j)) return false;

    return true;

}


/*
 * Checks whether v is not adjacent to every vertex in vertex set s.
 * NOTE: assumes s is sorted.
 * WARNING: this assumes that v is not contained in s
 */
bool Graph::is_completely_independent_from(const size_t v,
                                           const std::vector<size_t> & s) const {
    // If s is empty, return TRUE -- note, this is a design choice:
    // we want this function to reflect whether there are no edges from v to s.
    size_t s_size = s.size();
    if ( s_size == 0 ) return true;

    const std::vector<size_t> & v_neighborhood = get_neighbors_vector(v);
    size_t v_size = v_neighborhood.size();

    if ( v_size == 0 ) return true;

    // If v's neighborhood too large, can't be independent from s
    if ( v_size > this->num_vertices - s_size ) return false;

    // if range of v's neighbs totally outside range of s, return true
    if ((v_neighborhood.back() < s.front()) || (v_neighborhood.front() > s.back())){
        return true;
    }

    // Now we know their ranges intersect.
    // Can use pigeonhole principle if the two sets have too many elements
    // than can fit outside the intersection of their ranges.
    // But only bother to do this if the set sizes are large,
    // otherwise it isn't worth the extra flops.
    if ( std::min(s_size, v_size) > 10 ) {
        int s_lower = (int) s.front();
        int v_lower = (int) v_neighborhood.front();
        int s_upper = (int) s.back();
        int v_upper = (int) v_neighborhood.back();
        int common_range_lower_bound = std::max( s_lower, v_lower );
        int common_range_upper_bound = std::min( s_upper, v_upper );

        int min_number_elements_in_common_range = s_size - (s_upper - s_lower) + v_size - (v_upper - v_lower);

        // Check if sets have so many elements they must have common element:
        // nj - (L-lj + uj - U) is smallest num elements from Sj in common range
        // U-L+1 is exact size of common range.
        if (min_number_elements_in_common_range > (common_range_lower_bound - common_range_upper_bound + 1)) {
            return false;
        }
    }

    // ranges of s and v_neighborhood intersect, must inspect for edges.
    for (size_t j : s) {
        if (has_edge(v,j)) return false;
    }

    return true;

}
