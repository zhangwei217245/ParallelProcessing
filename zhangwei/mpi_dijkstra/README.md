# Project Introduction

## Team members: 

**Wei Zhang** and **Ali Nosrati**

## Description: 

In this project, we were required to parallelized dijkestra algorithm by 
using `MPI_Send` and `MPI_Recv` and by using an algorithm with time complexity 
equal to N^2/P (which P is the number of processes and N is the number of the
nodes of the graph)


## Overview of Algorithm 
Two parts of the dijkestra algorithm can be parallelized:

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
For parallelizing the chooseVertex function the master process (rank_0) provides
all the processes with required data by MPI_Send, and all the processes (including master process)  
will calculate a local minimum of their own and send the position for this local minimum back to master process, 
then the master process receives all those local minimum positions from all the processes, and by applying offset on 
the positions returned by each process and therefore comparing the elements in `dist[]` array indicated by the positions, 
the master process is able to eventually determine the global minimum element in `dist[]` array and its related position `j`.

The required data for each process contains:

		1. part of the `dist[]` array whose length equals to n/p (n = length of `dist[]` array, p = number of processes)
		2. part of the `found[]` array whose length equals to n/p (n = length of `found[]` array, p = number of processes)
    
**The 2nd part:** The for loop which is responsible of calculating the minimum and updating the
`dist[]` array
```
#!c
for (i = 0; i < n; i++){
    if (!(found[i])){
        dist[i] = min(dist[i], dist[j]+edge[j][i]);
}
```
For parallelizing this part of the code (for loop), the master process (rank_0) has
to send required data to all the processes including itself. The required data for each process contains:

		1. A part of the `dist[]` array whose length n/p (n = the length of `dist[]` array, p = number of processes)
		2. A part of the `found[]` array whose length n/p (n = the length of `found[]` array, p = number of processes)
		3. `dist[j]` (which is the current global minimum distance among all distances from SOURCE to all the other nodes)
		4. The `j`th row of edge[][] matrix (which is length of n/p, n = the length of `dist[]` array, p = number of processes)

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
make run NUM=n ARGS="-load 4 -src 0"
```

- **NUM** here is used to specify the number of processes, **n** here can be any integer number.
- **ARGS `-load`**: you can pass the load for each process by argument `-load` which means the number of columns in the matrix each process will be in charge of.
- **ARGS `-src`**: you can pass the SOURCE by argument `-src` to specify the `SOURCE` node.
- The size of the matrix would be (NUM*load) * (NUM*load), namely, NUM*load is both the width and height of the matrix.



# Observing the output of the program

Simply execute the following once you
successfully run the program.

```
#!sh
more output/*
```

# All-in-one project building

Simply run the command below:
```
#!sh
make viewrst NUM=n ARGS="-load 4 -src 0"
```
And this command will clean the project, compile the program, run it and show you the output with `more` utility.


