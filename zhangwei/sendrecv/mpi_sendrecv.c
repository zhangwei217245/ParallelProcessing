#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char** argv)
{
  // Variable Declarations
  int rank, size;
  MPI_Request *requestList,requestNull;
  MPI_Status  status;
  // Start MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  //printf("%d of %d\n",rank, size);
  if( rank == 0 )
  {
    // Process Zero
    int dataOut=13, dataIn, pr;
    requestList =(MPI_Request*)malloc((size-1)*sizeof(MPI_Request));
    for(pr=1;pr<size;pr++)
    {
      // MPI_Isend looks better here because a send request should  not
      // block the next send operation in the loop!
      // Tag=0 refers to a message from the process zero to the other processes.
      MPI_Isend(&dataOut,1,MPI_INT,pr,0,MPI_COMM_WORLD, &requestNull);
    }
    // We do NOT  need to wait for the MPI_ Isend(s), it is the job of the receiver  processes.
    for(pr=1;pr<size;pr++)
    {
      // Tag=1 refers to messages to the process zero from the rest of the processes.
      MPI_Irecv(&dataIn,1,MPI_INT,pr,1,MPI_COMM_WORLD,&(requestList[pr-1]));
    }
    int prW;
    for(prW=1;prW<size;prW++)
    {
       int index; // Index of the request array element that is completed.
       // Process zero is the receiver now, the rest of the processes are the senders.
       // We have size process but the processs zero is the master process and it is waiting
       // for the messages from the rest.
       MPI_Waitany(size-1, requestList, &index, &status);
       printf("From the process%d: %d\n", index+1, dataIn);
    }
  }
  else
  {
    // Receiver Process
    int message;
    //MPI_Request request;
    MPI_Status  status;
    // We do not need Irecv here because we need to wait in order to proceed anyway...
    MPI_Recv(&message,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
    // Modify the Message
    message =message*rank;
    // Send the message back to the process zero
    MPI_Send(&message,1,MPI_INT,0,1,MPI_COMM_WORLD);
  }
  MPI_Finalize();
return 0;
}
