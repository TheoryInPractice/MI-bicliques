# MI-bicliques
Algorithms for enumerating maximal bicliques, both in the induced and non-induced settings. 
Several approaches enumerate maximal induced bicliques; OCT-MIB and OCT-MIB-II are designed for graphs which are "near bipartite" (described in Kloster et al. 2018 and Sullivan et al. 2019); LexMIB is an improved version of the algorithm described in Dias et al. 2005; Enum-MIB is designed for general graphs and is not tailored for a specific class (Sullivan et al. 2019).
We also provide algorithms which enumerate maximal (non-induced) bicliques; MICA is our implementation of the approach described in Alexe at al. 2004; OCT-MICA refines the previous method to be optimal on "near bipartite" graphs (Sullivan et al. 2019).

## Table of Contents

README
* [Installation](#installation)
* [Command line usage](#command-line-usage)
* [Reproducing results from paper](#reproducing-results-from-paper)
* [Data formatting](#data-formatting)
* [Citation information](#citation-information)
* [License](#license)
* [Acknowledgements](#acknowledgements)

## Installation

The MI-bicliques code-base is set up for compilation using Cmake and Makefile.
Before starting this compilation process, check that Cmake version 3.9.0 or newer is installed, and GNU Make 4.1 or newer is installed;
the code has not been tested with older versions of Cmake or Make.

After those installations, run the following commands from the repo root directory:
1. run `cmake .` to generate the Makefile and necessary auxiliary files.
4. run `make` to compile the code and testing suite.
5. run `ctest` to run the testing suite. The 98 tests take well under a minute total on a standard desktop machine, and no tests should fail.

## Command line usage

Required arguments:
  * `-a` --- specify which algorithm to run. Options for biclique enumeration: `o` (OCT-MIB), `l` (LexMIB), `i` (OCT-MIB-II), `n` (Enum-MIB), `m` (MICA), `t` (OCT-MICA). Options that are not biclique enumerators: `b` (test whether the graph is bipartite via BFS) and `c` (counts the number of connected components via BFS).
  * `-i` --- specify the path to and filename of a graph (see "Data Formatting" below)

Optional arguments for all algorithms
  * `-h` --- Show a help message and exit.
  * `-t [TIMEOUT]`  --- Set a timeout for the algorithm.
  * `-c` --- Run in count-only mode; Do not output the bicliques, only count how many there are.
  * `-p [FILE_PATH]` --- Run in print mode; write the bicliques to file. Specify the path to and name of the file where the results should be stored. The output file is formatted such that each line is a single biclique with vertices comma-separated and sorted in ascending order.
  * `-l [FILE_PATH]` --- Print to file some statistics about the performance of the algorithm, including the runtime of algorithm components and the number of MIBs found. Specify the path to and name of the file where the results should be stored.

Optional argument for just OCT-MIB:
  * `-o [DECOMPOSITION_PATH]`  --- Specify the path to and filename of an OCT-decomposition for the input graph (see [Data formatting](#data-formatting) below).

### Example usage: OCT-MIB

The call
```
$>./bicliques -a o -i ./test/test_graph.txt -p octmib_results.txt
```
runs our algorithm OCT-MIB on a 7 node graph in our testing suite, and outputs the bicliques found to a file named `octmib_results.txt`.
The output to the screen should look like this:
```
# Running algorithm o
# Reading graph from ./test/test_graph.txt
# Printing MIBs to octmib_results.txt
# Starting algorithm OCT-MIB
# Graph has 1 connected components. CC ran in 3e-06
#	OCT in CC has size 2
#	L in CC has size 3
#	R in CC has size 2
```
and the file `octmib_results.txt` should look like this:
```
0,1,3,
1,2,3,4,
2,3,4,6,
0,2,4,
2,4,5,
1,3,5,
3,5,6,
```

To run the algorithm in count-only mode, instead run
```
$>./bicliques -a o -i ./test/test_graph.txt -c
```

### Example usage: LexMIB

The call
```
$>./bicliques -a l -i ./test/test_graph.txt -p lexmib_results.txt
```
runs the algorithm LexMIB on a 7 node graph in our testing suite, and outputs the bicliques found to a file named `lexmib_results.txt`.
The output to the screen should look like this:
```
# Running algorithm l
# Reading graph from ./test/test_graph.txt
# Printing MIBs to lexmib_results.txt
# Starting algorithm LexMIB
```
and the file `lexmib_results.txt` should look like this:
```
0,1,3,
0,2,4,
1,2,3,4,
1,3,5,
2,3,4,6,
2,4,5,
3,5,6,
```

## Reproducing results from paper

The subdirectory `reproducibility/` contains code for generating the datasets
used in the experiments of our accompanying paper.


## Data formatting

The MI-bicliques codes can read graphs from txt files in two formats: adjacency list, and edgelist.
For both formats, the first line of the txt file should list the number of nodes then the number of edges.
The files should be space or tab delimited (not comma delimited).

Our algorithm OCT-MIB can use a user-prescribed OCT-decomposition by passing an additional file in to the algorithm.
A file specifying an OCT-decomposition should consist of two lines (space or tab delimited):
the first row should list the vertices in the OCT set, and the second row should list the vertices in one of the other two partitions.
The code will infer the third partition given the first two.

## Citation information

**Important**: MI-bicliques is *research software*, so you should cite us when you use it in scientific publications! Please see the [CITATION](./CITATION) file for citation information.

## License

MI-bicliques is released under the BSD license; see the [LICENSE](./LICENSE) file.
Distribution, modification and redistribution, and incorporation into other software is allowed.


## Acknowledgments

Development of the MI-bicliques software package was funded in part by
the [Gordon & Betty Moore Foundation Data-Driven Discovery Initiative](https://www.moore.org/programs/science/data-driven-discovery),
through a [DDD Investigator Award](https://www.moore.org/programs/science/data-driven-discovery/investigators)
to Blair D. Sullivan ([grant GBMF4560](https://www.moore.org/grants/list/GBMF4560)).
