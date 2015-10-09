# Project Introduction

## Team members: 

Wei Zhang and Ali Nosrati

## Description: 

In this project, we were required to implement barrier (synchronization
of multiple processes) by using `MPI_Send` and `MPI_Recv` and by using an
algorithm with time complexity less than 10*LogP (P is the number of the
processes).

There are several different algorithms available to perform a synchronization
of multiple processes, which are suitable for distributed memory architectures 
and message-passing-based computer systems. The algorithm that we
have used to implement barrier is called "dissemination" and it can work perfectly 
with any random number of the processes (There is no limitation for algorithm to 
work properly only for the number of processes (P), which P is a power of two). 
For more information about dissemination you can check 

DEBRAH ENGSEN, RAPHAEL FINKEL and UDI MANBER, __Two Algorithms for Barrier Synchronization__ , March 1988.

## Overview of Algorithm 
By using dissemination algorithm, every processes cannot continue working and 
have to wait until they receive some arbitary data from all other processes.
The superiority of dissemination algorithm is that it can synchronize all 
the processes in only log(P) steps.

In every steps, every processes call MPI_Send to send some arbitary data to their
corresponding receiver (dst) first, and then call MPI_Recv for receiving the data 
from corresponding sender (src). From the view of dst process, src is the source 
address where the message comes from.
Selecting src and dst processes in each steps, is the key that let us synchronize 
multiple processes in log(P) steps. For this purpose, we have to especify src and dest
so that 'dest-src = two to the power of current step'. (and we also have to wrap around the
dest process id, if necessary).


For example, Imagine that we have 5 processes (With process id form 0 to 4)

In the 1st step (Step number = 0), dst-src = 2<sup>0 = 1:

|Sender         |   Receiver    |
|:--------------|--------------:|
|process rank_0 | process rank_1| 
|process rank_1 | process rank_2|
|process rank_2 | process rank_3|
|process rank_3 | process rank_4|
|process rank_4 | process rank_0|

(In this step, rank_0 send some data to rank_1 and also wait until it receive
some data from rank_4 and so on)


In the 2nd step (Step number = 1), dst-src =  2^1 = 2:

|Sender         |   Receiver    |
|:--------------|--------------:|
|process rank_0 | process rank_2| 
|process rank_1 | process rank_3|
|process rank_2 | process rank_4|
|process rank_3 | process rank_0|
|process rank_4 | process rank_1|


In the 3rd step (Step number = 2), dst-src = 2^2 = 4:

|Sender         |   Receiver    |
|:--------------|--------------:|
|process rank_0 | process rank_4| 
|process rank_1 | process rank_0|
|process rank_2 | process rank_1|
|process rank_3 | process rank_2|
|process rank_4 | process rank_3|


And right after 3 steps all the process will be informed whether the other process
have reached the barrier or not.


In this part of code you can see how we have determined the src process number (that 
arbitrary data have to be received from) and the destination process number (which we have to send 
some arbitrary data to it) in every different steps.

```
#!c
int totalSteps = ceiling(lg((double) world_size));
for (i = 0; i < totalSteps; i++){
    int pow_2 = (int)pow((double)2, (double)i);
    int dst = (world_rank + pow_2) % world_size;
    int src = (world_rank - pow_2 + world_size) % world_size;
    printf("[%s] %d -> %d -> %d\n", getTimeString(),src, world_rank, dst);
    MPI_Send(&i, 1, MPI_INT, dst, 0, MPI_COMM_WORLD);
    MPI_Recv(&x, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &status);
}

```

## Overview of testing code

For testing part, we generated three different type of processes with three different 
length of execution. After using dissemination algorithm you can see in the output
file that every processes have to wait until all other processes reach their barriers. 

```
#!c
    printf("[%s]: Process #%d STARTS.\n", getTimeString(), world_rank);
    
    // Each process will carry out some calculation or operations, which takes different times.
    if (world_rank % 3 == 0){ 
            sleep(5);
    } else if (world_rank % 3 == 1) {
            sleep(3);
    } else {
            sleep(1);
    }
    
    printf("[%s]: Process #%d is ENTERING barrier.\n", getTimeString(), world_rank);

    // Call the barrier function here
    my_barrier(MPI_COMM_WORLD);

    // print the message after the barrier, by each process.
    printf("[%s]: Process #%d: PASSED barrier.\n", getTimeString(), world_rank);
```

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
make run NUM=n ARGS="-send a -wait b -fill c"
```

- Note: here, argument **NUM** here is used to specify the number of processes,
**n** here can be any number out of 1,2,3,4,6,12.

- **ARGS** is for passing arguments to the parallel program. We use **ARGS**
for passing send mode and wait mode to the program. For example,

```
#!sh
make run NUM=2 ARGS="-send 1 -wait 0"
```
This means that the program will send messages in batch mode, and detect
the arrival of messages with MPI_Test.

Here are all the acceptable arguments for this program, and their meaning.

```
#!c
Arguments:
-wait : 
0: calculate with MPI_Test; 
1: calculate with MPI_Waitany.
-send : 
0: send each row right after it is generated; 
1: send rows for particular process after they are
generated totally.
-fill : 
Fill the array with: 
0 - random integer mod row_count+1; 
1 - random integer; 
2 - row_count.
```

# Observing the output of the program

Simply execute the following once you
successfully run the program.

```
#!sh
more output/*
```