# Project Introduction

## Team members: 

**Wei Zhang** and **Ali Nosrati**

## About due date:

Permitted by Dr. Zhuang, the due date is changed to Nov 2nd, 2015, before 12.00pm.

## Description: 

In this project, we were required to parallelize the Floyd algorithm to
calculate all the shortest paths between every two-paired nodes of a graph (by
using the graph's adjacency matrix which is a $`N \times N`$ matrix (N is the number
of the nodes of that graph))

There are several different method to parallelize Floyd algorithm:

1st method, Row-Based Method: In this method, we will parallelize the matrix
row by row. The master process (i.e. `rank_0`) send the required chunk of data 
(Including some rows dedicated to that process and other necessary data) to 
every processes. Each process calculate the Floyd algorithm for its corresponding 
chunk of data and after all, send the updated data back to the master process to 
be merged and be printed in the output. In this method of parallelizing we have 
to achieve the time complexity of $`\mathcal{O}(n^2log\mathcal{P})`$ .

2nd Method, Grid-Based Method: In this method, we will parallelize the matrix
in like a grid (i.e. three by three). The master process (i.e. `rank_0`) send 
the required chunk of data (Including a submatrix dedicated to that process 
and other necessary data) to every processes. Each process calculate the Floyd 
algorithm for its corresponding chunk of data and after all, send the updated 
data back to the master process to be merged and be printed in the output. 
In this method of parallelizing we have to achieve the time complexity of 
$`\mathcal{O}(\frac{n^2}{\sqrt{P}} \times log\mathcal{P})`$ and we actually achieved the 
time complexity of $`\mathcal{O}(\frac{n^2}{\sqrt{P}} \times log\sqrt{P})`$.





## Overview of Algorithm 
For this assignment we decide using Grid-base Method to parallelize the Floyd 
algorithm.  For this purpose, first of all, We have to check the dimension of 
the adjacency matrix to see if it is divisable by root square of P (P
is the number of the processes). Then the `rank_0` process have to send to every
process their corresponding chunk of data which is a sub-matrix of adjacency
matrix. The size of each sub-matrix is calculated by $`\frac{N}{\sqrt{P}}`$ (which we call 
it the grid-size). Since every process need the data from the last iteration while updating
the value of there own sub-matrix, it's better to have a copy of their sub-matrix. However, having 
observed that only the data in `k`th column and `k`th row should be cached for calculating and updating 
the value, to be more memory efficient, we only provide all precesses with the coresponding part of the
`k`th row and `k`th column. The process which contains the current `k`th row or `k`th column, 
have to broadcast the $`\sqrt{P}`$ parts of the data vertically or horizantally. To do this we
used `MPI_Comm_split()` function to clolorized the horizental and verticall groups of
processes. 
```
#!c
		// Create row-based communicator and column-based communicator.
		int r = world_rank / sqrt_p;
		int c = world_rank % sqrt_p;
		MPI_Comm row_comm, col_comm;
		MPI_Comm_split(MPI_COMM_WORLD, r, world_rank, &row_comm);
		MPI_Comm_split(MPI_COMM_WORLD, c, world_rank, &col_comm);
		int row_rank, row_size, col_rank, col_size;
		getSizeAndRank(&row_size,&row_rank, row_comm);
		getSizeAndRank(&col_size,&col_rank, col_comm);

```

Now we can broadcast the data as we expected to do. Namely,
In each iteration of the `k` loop, we broad cast part of the data in the `k`th row vertically
and the `k`th column horizontally.
```
#!c
				// The index for column and row which contains the sender processes.
				int si = k / grid_size;
				// Broadcast the kth row vertically:
				if (col_rank == si) {
						for (j = 0; j < grid_size; j++){
								horz_buff[j] = buf[k % grid_size][j];
						}
				}
				MPI_Bcast(horz_buff, grid_size, MPI_INT, si, col_comm);


				// Broadcast the kth column horizontally:
				if (row_rank == si) {
						for (j = 0; j < grid_size; j++){
								vert_buff[j] = buf[j][k % grid_size];
						}
				}
				MPI_Bcast(vert_buff, grid_size, MPI_INT, si, row_comm);


```


Then every process begin calculating and updating its own 
sub-matrix based on the Floyd algorithm and by running this piece of code:
```
#!c
for ( i = 0 ; i < grid_size ; i++){
		for (j = 0; j < grid_size; j++){
				buf[i][j] = min(buf[i][j], safesum(vert_buff[i], horz_buff[j]));
		}
}
```
Here, the vert_buff is the part inside the `k`th column, while horz_buff is the counterpart for `k`th row.

When k=N, which is the last step, everyprocess contains the shortest path data
about their sub-matrix, so the only thing that we need to do is to collect the
data back to the master. Afterwards, the master process, after collecting all the data, 
print the matrix out to the standard output.


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
make run NUM=p ARGS="-msize n"
```

- Note: here, argument **NUM** here is used to specify the number of processes,
**p** here must be a perfect square number. 
- `-msize` is for specifying the matrix size. n should be divisible by $`\sqrt{P}`$,
  and the matrix size is going to be $`n \times n`$;

Here are some simple test cases:
```
#!sh
make run NUM=4 ARGS="-msize 4"
make run NUM=9 ARGS="-msize 12"
make run NUM=16 ARGS="-msize 28"
```


# Observing the output of the program

Simply execute the following once you
successfully run the program.

```
#!sh
more output/*
```
