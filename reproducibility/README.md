# Bicliques experiments - reproducibility

To create the datasets for our experiments, we used a random graph generator with multiple tunable parameters.
In this file we explain how to use the graph generators, as well as the specific parameter settings we used for each of our experiments.

## Installation

To compile our generator scripts we used gcc (version 5.3.1).
We compiled with the following command:

`gcc OCTgenerator.c -lm -O3 -o generator`

The same command can be used to compile `OCTgenerator_pm.c`.

## Using the graph generators

The files `OCTgenerator.c` and `OCTgenerator_pm.c` can generate graphs with prescribed OCT structure.

The code `OCTgenerator.c` takes in 9 arguments in the following order:

* m - the number of nodes in the left partition
* n - the number of nodes in the right partition
* mean - the expected average number of neighbors in the left partition over the right partition nodes
* std - the expected standard deviation of the number of neighbors in the left partition over the right partition nodes
* oct - the number of nodes in the OCT set
* octoct - the expected edge density within the OCT set (from 0 to 1)
* octg - the expected edge density between the OCT set and the non-OCT set nodes (from 0 to 1)
* octgcv - the expected coefficient of variance[1] of the number of non-OCT neighbors over the OCT nodes
* seed - used for reproducibility

[1] coefficient of variance is the standard deviation divided by the mean

The code `OCTgenerator_pm.c` takes in the same arguments, but always outputs an oct decomposition which is a perfect matching.


### Example usage

To a generate a graph with 20 nodes in the left partition, 10 nodes in the right partition, 5 nodes in the OCT set, expected edge density between the left and right partition equal to 10%, expected edge density in the OCT set equal to 1%, expected edge density between the OCT set and other partitions equal to 5%,  cv of OCT-{L, R} and L-R degrees equal to 0.5, and seeded with value 1234, run the following command:

`./generator 20 10 2 1 5 0.01 0.05 0.5 1234`

Note that all nodes in the right partition having an expected number of left neighbors equal to 2 of a possible 20 implies an expected edge density of 10%. Since this average number of neighbors is 2, setting the standard deviation equal to 1 yields a cv value of 0.5.

## Reproducing our datasets

We use bracketed lists ([value1, value2, ...]) to represent all of the values a parameter takes for a set of graphs.
If multiple parameters have a range of values, then a graph is made for each combination of parameter values.

### Coefficient of Variance between L and R

Generate graphs with the following parameter values:

Graphs where m + n = 200
* m= 182
* n= 18
* mean= 9.1
* std= [2.73, 3.185, 3.64, 4.095, 4.55, 5.005, 5.46, 5.915, 6.37, 6.825, 7.28, 7.735, 8.19, 8.645, 9.1, 9.555, 10.01, 10.465, 10.92]
* oct= [10, 14]
* octoct= 0.05
* octg= 0.05
* octgcv= 0.5
* seed= [22522, 69209, 93577, 51386, 98111]

Graphs where m + n = 1000

* m= 909
* n= 91
* mean= 45.45
* std= [6.8175, 7.95375, 9.09, 10.22625, 11.3625, 12.49875, 13.635, 14.77125, 15.9075, 17.04375, 18.18, 19.31625, 20.4525, 21.58875, 22.725, 23.86125, 24.9975, 26.13375, 27.27]
* oct= [10, 19]
* octoct= 0.05
* octg= 0.05
* octgcv= 0.5
* seed= [22522, 69209, 93577, 51386, 98111]


### Edge density between L and R

(^): When `mean/m = 0.05`, we only use one value of `octoct`.

Graphs where m + n = 150
* m= 75
* n= 75
* mean= [3.75, 7.5, 11.25, 15, 18.75]
* std= mean/2
* oct= 5
* octoct= [0.05, mean/m]
* octg= mean/m
* octgcv= 0.5
* seed= [22522, 69209, 93577, 51386, 98111]

Graphs where m + n = 200
* m= 100
* n= 100
* mean= [5, 10, 15, 20, 25]
* std= mean/2
* oct= 5
* octoct= [0.05, mean/m]  (^)
* octg= mean/m
* octgcv= 0.5
* seed= [22522, 69209, 93577, 51386, 98111]

Graphs where m + n = 300
* m= 150
* n= 150
* mean= [7.5, 15, 22.5, 30, 37.5]
* std= mean/2
* oct= 5
* octoct= [0.05, mean/m]  (^)
* octg= mean/m
* octgcv= 0.5
* seed= [22522, 69209, 93577, 51386, 98111]


### Size and balance of L and R

Graphs where m + n = 200
* (m,n)=[(195,5), (182,18), (133,67), (100,100)]
* mean= m*0.05
* std= mean/2
* oct= [10, 14]
* octoct= 0.05
* octg= 0.05
* octgcv= 0.5
* seed= [22522, 69209, 93577, 51386, 98111]

Graphs where m + n = 1000
* (m,n)=[(990,10), (909,91), (667,333), (500,500)]
* mean= m*0.05
* std= mean/2
* oct= [10, 19]
* octoct= 0.05
* octg= 0.05
* octgcv= 0.5
* seed= [22522, 69209, 93577, 51386, 98111]

### Size of OCT

* m= 909
* n= 91
* mean= 45.45
* std= 22.725
* oct= [0, 2, 4, 6, 8, 10, 12, 14, 16, 18]
* octoct= 0.05
* octg= 0.05
* octgcv= 0.5
* seed= [22522, 69209, 93577, 51386, 98111]


### Density between OCT and {L, R}

* m= 909
* n= 91
* mean= 45.45
* std= 22.725
* oct= [10, 19]
* octoct= 0.05
* octg= [0.01, 0.03, 0.05, 0.07, 0.09]
* octgcv= 0
* seed= [22522, 69209, 93577, 51386, 98111]

### Density within OCT

* m= 909
* n= 91
* mean= 45.45
* std= 22.725
* oct= [10, 19]
* octoct= [0.01, 0.03, 0.05, 0.07, 0.09, 0.11, 0.13, 0.15, 0.17, 0.19]
* octg= 0.05
* octgcv= 0
* seed= [22522, 69209, 93577, 51386, 98111]

### Independent OCT sets

* m= 9091
* n= 909
* mean= [45.45, 90.91]
* std= mean/2
* oct= [5, 10, 25]
* octoct= 0
* octg= mean/m
* octgcv= 0
* seed= [22522, 69209, 93577, 51386, 98111]

### Perfect Matching OCT Sets
For this experiment we used a version of the generator which forces the OCT set to be a perfect matching.
The code still requires an input value for `octoct`, but the value passed in is not actually used.

Graphs where m + n = 300
* m= 150
* n= 150
* mean= 7.5
* std= 3.75
* oct= 14
* octoct= 0
* octg= 0.05
* octgcv= 0
* seed= [22522, 69209, 93577, 51386, 98111]

Graphs where m + n = 600
* m= 300
* n= 300
* mean= 15
* std= 7.5
* oct= 16
* octoct= 0
* octg= 0.05
* octgcv= 0
* seed= [22522, 69209, 93577, 51386, 98111]
