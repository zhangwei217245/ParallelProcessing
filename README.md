* Clean Your distribution

`make clean`

* Build Your Executables

`make`

* Run Your Program

`make run NUM=2/3/4/5/.../n ARGS="..."`

Here, argument **NUM** here is used to specify the number of processes.
**ARGS** is for passing arguments to the parallel program.
In **zhangwei/bigarray**, we use **ARGS** for passing send mode and wait mode to the program. For example,
`make run NUM=2 ARGS="-send 1 -wait 0"` means that the program will send messages in batch mode, and detect the arrival of messages with MPI_Test.

| value | -send | -wait |
|-------+-------+-------|
|   0   |  seq  |  test |
|   1   | batch |waitany|
