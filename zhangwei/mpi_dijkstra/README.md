# Project Introduction

## Team members: 

**Wei Zhang** and **Ali Nosrati**

## Description: 

In this project, we were required to parallelized dijkestra algorithm by 
using `MPI_Send` and `MPI_Recv` and by using an algorithm with time complexity 
equal to N^2/P (which P is the number of processes and N is the number of the
nodes of the graph)


## Overview of Algorithm 
Two part of the dijkestra algorithm can be parallelized:

**The 1st part:** The chooseVertex() function
```
#!c
chooseVertex(int *dist, int n, int *found){
int i, tmp, leastPosition=0;
int least = INFTY;

    for (i = 0; i < n; i++){
        tmp = dist[i];
        if ((!found[i]) && (tmp < least)){
            least = tmp;
            leastPosition = i;
        }
    }
return leastPosition;
}
```
For parallelizing the chooseVertex function the head node (rank_0) provide
the every other processes with required data to calculate a local minimum,
then the head node receives the calculated local minimum from all other
processes and will calculated the global minimum.


    
**The 2nd part:** The for loop which is responsible of calculating the minimum and update the
dist[] array
```
#!c
for (i = 0; i < n; i++){
    if (!(found[i])){
        dist[i] = min(dist[i], dist[j]+edge[j][i]);
}
```
For parallelizing this part of the code (for loop), the head process (rank_0) have 
to send required data to every other processes. The required data contains:
1)Full found[] array
2)A part of the dist[] array (as much as chunkSize)
3)dist[j] (which is the current global minimum)
4)One row of edge[][] matrix (which contains the requisite data for every processes 
to calculate the minimum and to update the dist[] array)

Every process (including the rank_0 itself) have to calculated the minimum and
also have to update the part of the dist[] array which they are responsible
for. Then the head node receives every different part of the dist[] array from
all processes and calculate the new (current) global minimum (j). Then the
process will go on, until the all elements of found[] array will be 1. On that
situation the answer is the merged dist[] array.

## Overview of testing code

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
