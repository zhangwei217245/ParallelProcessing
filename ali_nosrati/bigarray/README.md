# Project Introduction
    
## Team members: 

Wei Zhang, Ali Nosrati.

## Description: 

In this project, we were required to implement message sending and receiving using MPI. The messages are *1200* rows of data with each row containing *1024* integer numbers, which are evenly sent to different processes by one of them. All the processes receives the same amount of rows, and calculate the average of each row and print it out.

During the data sending process, the data generating phase should be overlapped with the data sending phase, which could be implemented by invoking *MPI_Isend* function. While during the data receiving processes, the data receiving phase for each row should be overlapped with the calculation of the average of a certain row.

## Overview of data distribution policy:

There are *1200* rows in all, and there will be *n* processes running in parallel with one of them generating the data of *1200* rows and sending them out to all the *n* processes evenly. We let process *0* generate and the send the data, all the processes including process *0* will receive the data and calculate the average number of each row. So, each process should receive *1200/n* rows of data, namely, we separate the rows of data into *1200/n* groups, and for a certain group *p* (which will be sent to process *p*), the indices of the rows inside this group should range from *(p-1)\*1200/n* to *(p\*1200/n)-1*. 

## Overlapping while generating and sending the data.

We offer two modes for generating and sending the data, which are:

### Sending a row right after generating it.

For simplicity, we demonstrate the pseudocode as below:

```
#!c

for (p = 0 to n) {
    for (i = (p-1)*1200/n to (p*1200/n)-1 ){
        generating row[i];
        calling MPI_Isend for sending row[i] to process p;
    }
}
```

In this method, since MPI_Isend is asynchronous, it won't block the generating of the next row, namely, the generating of the next row *r+1* can overlap with the sending phase of the current row *r*.

### Sending a group of rows after generating these rows (we call it batch mode)

For simplicity, we also demonstrate the pseudocode here:

```
#!c
for (p = 0 to n) {
    for (i = (p-1)*1200/n to (p*1200/n)-1 ){
        generating row[i];
    }
    for (i = (p-1)*1200/n to (p*1200/n)-1 ){
        calling MPI_Isend for sending row[i] to process p;
    }
}
```
In this method, since *MPI_Isend* is asychronous, multiple *MPI_Isend* calls won't block the invocation of themselves, though the actual data sending phase of a certain *MPI_Isend* call could be blocked by others' data sending phase. Then, where is the overlapping? It's interesting to note that while the data sending for process *p* is working in background, the generating of the rows for process *p+1* could be carried out simultaneously. This is a coarser level of overlapping which could happen between the generation of a bunch of data and the sending phase for another bunch of data.

## Overlapping while receiving the rows and calculating the average for each row.

Bacially, this can be implemented by receiving row *r+1* while calculating the average for row *r*. 

However, again, since the *MPI_Irecv* function offers the capability of asynchronous execution (though the actual data receiving phase may be blocked due to limited I/O or memory data path, the invocation of multiple *MPI_Irecv* function calls can happen at the same time), in each process, we can initial exactly *1200/n* *MPI_Irecv* calls at the beginning for receiving *1200/n* rows ranging from *(p-1)\*1200/n* to *p\*1200/n-1* (for process *0*, this can happen even before the data is generated and sent, since the MPI_Irecv is also asynchronous). Then we may invoke *MPI_Test* for multiple times in a loop to detect all those *1200/n* *MPI_Request* references to see whether any of them is finished. Certainly, invoking *MPI_Test* in a loop is equivalent to invoking *MPI_Waitany* for a list of *MPI_Request* references.

Here is the pseudocode:

```
#!c
// for initilizing MPI_Irecv calls

for (i = (p-1)*1200/n to p*1200/n-1) {
    call MPI_Irecv for receiving row[i] in process p, associated with MPI_Request req[i];
}

// any reasonable procedure here
....

// calculate the average number once any *MPI_Request* for *MPI_Irecv* call is finished.

while (the number of rows calculated &lt; 1200/n) {
    // MPI_Requests is an array of MPI_Request accociated with different MPI_Irecv calls.
    if ( using MPI_Waitany ){
        MPI_Waitany(MPI_Requests, &index);
        calculate_avg(row[index]);

    }
    if (using MPI_Test) {
        for(i = 0 to MPI_Requests.length - 1){
            MPI_Test(MPI_Requests[index], &flag);
            if(flag){
                calculate_avg(row[index]);
            } 
        }
    }
}
```

So, still, due to the limited resources of data path, the actual receiving phase of a certain MPI_Irecv function may be blocked by another, however, it is indisputable that once any of them is finished, the calculation for average of the current row *r* will overlap with the actual receiving phase of another MPI_Irecv function for row *r+1*.


# To build the program:

Since it is seriously difficult to get access to a computing node via qlogin, we suggest you can directly access a certain computing node on cluster "hrothgar" by ssh. For example, ```ssh compute-1-1``` will enable you to access the computing node "compute-1-1".


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

- Note: here, argument **NUM** here is used to specify the number of processes, **n** here can be any number out of 1,2,3,4,6,12.

- **ARGS** is for passing arguments to the parallel program. We use **ARGS** for passing send mode and wait mode to the program. For example,

```
#!sh
make run NUM=2 ARGS="-send 1 -wait 0"
```
This means that the program will send messages in batch mode, and detect the arrival of messages with MPI_Test.

Here are all the acceptable arguments for this program, and their meaning.

```
#!c
Arguments:
 -wait : 
    0: calculate with MPI_Test; 
    1: calculate with MPI_Waitany.
 -send : 
    0: send each row right after it is generated; 
    1: send rows for particular process after they are generated totally.
 -fill : 
    Fill the array with: 
    0 - random integer mod row_count+1; 
    1 - random integer; 
    2 - row_count.
```