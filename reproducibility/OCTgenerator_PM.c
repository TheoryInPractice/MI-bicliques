/* Random Bipartite Graph Generator 
 * Input: 	m - the number of nodes in the left partition
 * 		n - the number of nodes in the right partition
 * 		mean - the average number of neighbors in the left partition of right partition nodes
 *		std - the standard deviation in the number of left neighbors of right partition nodes
 * 		oct - the number of nodes in the OCT set
 * 		octoct - the expected edge density within the oct set
 * 		octg - the expected edge density between the OCT set and the rest of the graph
 * 		seed - to seed the random graph generator, for replicability purposes
 *
 * Output: a bipartite graph with an OCT set, random, undirected, unweighted
 *
 * Biclique Project for ODE, U. of Tennessee and ORNL
 * Original Author: Yun Zhang
 * 
 * Original code updated by Kyle Kloster and Andrew van der Poel to include functionality
 * of adding an OCT set to a bipartite graph 
 * 
 * Originally Created: Oct 8, 2007
 * Last Updated: Oct 2018
 * 
 * 
 * Original version of this code by Yun Zhang, used under BSD 3-Clause license. 
 * This version of the code modified from the original by adding the option to create an OCT set.
 * Modified by Kyle Kloster and Andrew van der Poel.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*-----------------------------------------------------------------
 *  Function: box_muller()
 *    Implements the Polar form of the Box-Muller Transformation
 *    to generate a random variable with gaussian distribution
 *    of given mean and standard deviation
 *-----------------------------------------------------------------*/
float box_muller(float m, float s)	/* normal random variate generator */
{				                    /* mean m, standard deviation s */
  float x1, x2, w, y1;
  static float y2;
  static int use_last = 0;

  if (use_last) {		        /* use value from previous call */
    y1 = y2;
    use_last = 0;
  }
  else
  {
    do {
      x1 = 2.0 * drand48() - 1.0;
      x2 = 2.0 * drand48() - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );

    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;
    use_last = 1;
  }

  return( m + y1 * s );
}

/* -----------------------------------
 * Random bipartite graph generator
 * Function: output the edges in fly
 * ----------------------------------- */
void bigraph_generator(FILE *fp, int m, int n, float mean, float std, float oct, float octoct, float octg, float octgcv, int seed)
{
  int u, v, o, i, j, e=0;
  float w, d, p, density, deg, prob;
  

  // init random generator
  srand48(seed);
  
  // for every vertex v in V
  for (v = 0; v < n; v++) {
	// decide its degree using gaussian distribution with mean and variance
	d = box_muller(mean, std);
	p = d / (float) m;  // edge density
	
    for (u = 0; u < m; u++) {

      // output an edge with probability <= p
      w = drand48();
      if (w <= p) {
        e++;
        fprintf(fp, "%d\t%d\n", u, m+v);
      }

    }
  }

  // calculate oct degree stats
  float oct_mean, oct_std;
  oct_mean = (mean / (float) m) * (m + n);
  oct_std = octgcv * oct_mean;


  for (o = 0; o < oct; o++) {
	// decide the number of nbrs it has in the bipartite portion 
	deg = box_muller(oct_mean, oct_std);
	p = deg / (float) (m + n);

	//for every oct, non-oct vertex pair determine if they share an edge
	for (i = 0; i < (m+n); i++) {
	      // output an edge with probability <= p
	      w = drand48();
	      if (w <= p) {
		e++;
		fprintf(fp, "%d\t%d\n", i, o+m+n);
	      }
	}

  }

// make oct set a matching
  int half;
  half = oct/2;
  fprintf(stderr, "Half: %d OCT: %f\n", half, oct);

  for (o = 0; o < half; o++) {
	fprintf(stderr, "%d\t%d\n", o+m+n, half+o+m+n);
	fprintf(fp, "%d\t%d\n", o+m+n, half+o+m+n);

  return;
}


/* -----------------------------------
 * Driver
 * ----------------------------------- */
int main(int argc, char **argv)
{

  // initialize variables
  int n, m, oct;   
  float mean, std, octoct, octg, octgcv;  
  int seed;
  
  if (argc < 8) {
    fprintf(stderr, "Usage: %s m n mean std seed\n", argv[0]);
	fprintf(stderr, "  m - the expected number of nodes on one (left) side\n");
	fprintf(stderr, "  n - the expected number of nodes on the other (right) side\n");
	fprintf(stderr, "  mean - the mean of degree for right side vertices\n");
	fprintf(stderr, "  std - the standard deviation of degree for right side vertices\n");
	fprintf(stderr, "  oct - the # of oct nodes\n");	
	fprintf(stderr, "  oct-oct - the density within the oct set\n");
	fprintf(stderr, "  oct-G- the density between the oct set and bipartite graph\n");
	fprintf(stderr, "  the cv of the # of neighbors oct nodes have in the left and right side\n");
	fprintf(stderr, "  seed - the seed\n");
	fprintf(stderr, "\n");
    exit(1);
  }

  // get arguments
  m = atoi(argv[1]);  // number of left nodes
  n = atoi(argv[2]);  // number of right nodes
  mean = atof(argv[3]);	// mean of degree for right side
  std = atof(argv[4]);	// standard deviation of degree for right side
  oct = atof(argv[5]);	// number of oct nodes
  octoct = atof(argv[6]);	//density within the oct set
  octg = atof(argv[7]);		//density between the oct set and the rest of the graph
  octgcv = atof(argv[8]);	//cv of the # of neighbors oct nodes have in rest of graph
  seed = atof(argv[9]);		

  // generate a random bigraph with m,n,p
  bigraph_generator(stdout, m, n, mean, std, oct, octoct, octg, octgcv, seed);
  
  exit(0);
}

