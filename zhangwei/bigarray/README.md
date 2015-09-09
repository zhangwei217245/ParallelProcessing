* Clean Your distribution

`make clean`

* Build Your Executables

`make`

* Run Your Program

`make run NUM=2/3/4/5/.../n ARGS="..."`

Here, argument **NUM** here is used to specify the number of processes.

**ARGS** is for passing arguments to the parallel program.

In **zhangwei/bigarray**, we use **ARGS** for passing send mode and wait mode to the program. For example,

`make run NUM=2 ARGS="-send 1 -wait 0"` 

means that the program will send messages in batch mode, and detect the arrival of messages with MPI_Test.


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
