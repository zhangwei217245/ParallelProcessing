# Project Introduction

## Team members: 

**Wei Zhang** and **Ali Nosrati**

## Description: 

In this project, we were required to parallelize the Floyd algorithm to
calculate all the shortest paths between every two-paired nodes of a graph (by
using the graph's adjacency matrix which is a N*N matrix (N is the number
of the nodes of that graph))

There are several different method to parallelize Floyd algorithm:

1st method, Row-Based Method: In this method, we will parallelize the matrix
row by row. The master process (i.e. rank_0) send the required chunk of data 
(Including some rows dedicated to that process and other necessary data) to 
every processes. Each process calculate the Floyd algorithm for its corresponding 
chunk of data and after all, send the updated data back to the master process to 
be merged and be printed in the output. In this method of parallelizing we have 
to achieve the time complexity of (n^2logp)

2nd Method, Grid-Based Method: In this method, we will parallelize the matrix
in like a grid (i.e. three by three). The master process (i.e. rank_0) send 
the required chunk of data (Including a submatrix dedicated to that process 
and other necessary data) to every processes. Each process calculate the Floyd 
algorithm for its corresponding chunk of data and after all, send the updated 
data back to the master process to be merged and be printed in the output. 
In this method of parallelizing we have to achieve the time complexity of 
(n^2/sqrt(P)*logp) and we achieve the time complexity of (n^2/sqrt(P)*log[sqrt(P)]).


## Overview of Algorithm 
For this assignment we decide using Grid-base Method to parallelize the Floyd 
algorithm.  For this purpose, First of all, We have to check the dimension of 
the adjacency matrix to see if it is divisable by root square of P (P
is the number of the processes). Then the rank_0 process have to send to every
process their corresponding chunk of data which is a sub-matrix of adjacency
matrix. The size of each sub-matrix is calculated by N/sqrt(P)(which we call 
it the grid-size). Then every process began calculating and updating its 
sub-matrix based on the Floyd algorithm and by running these piece of code:
```
#!c
int k=0;
while (k < n){
    for ( i = 0 ; i < n ; i++)
        for (j = 0; j < n; j++){
            W[i][j] = min(W0[i][j], W0[i][k]+W0[k][j];
    }
    k++;
}
```
Since every process need the W0 from the last iteration, we have to provide
them with such that information. But to be more memory efficient, we only
provide all precesses with the coresponding part of the current Kth row and Kth
column. 
```
#!c
related code here!
```
The process which contains the current Kth row or Kth column, have to broadcast 
the sqrt(P) parts of the data vertically or horizantally. To do this we
used MPI_COMM_SPLIT() to clolorized the horizental and verticall groups of
processes. Now we can broadcast the data as we expected to do.

When k=N, which is the last step, everyprocess contains the shortest path data
about their sub-matrix, so the only thing that we need to do is to collect the
data back to the master. We do that duty by using this pease of code"
```
#!c
```

The master process, after collecting all the data, print the matrix out to the output.


# To build the program:

Since it is seriously difficult to get access to a computing node via qlogin,
we suggest you can directly access a certain computing node on cluster
"hrothgar" by ssh. For example, ```ssh compute-1-1``` will enable you to access
the computing node "compute-1-1".


## Do some cleaning before compiling.

```
#!sh
make clean
```

## Build the executables

```
#!sh
make
```

# Run the program


```
#!sh
make run NUM=n
```

- Note: here, argument **NUM** here is used to specify the number of processes,
**n** here can be any integer number.


# Observing the output of the program

Simply execute the following once you
successfully run the program.

```
#!sh
more output/*
```
