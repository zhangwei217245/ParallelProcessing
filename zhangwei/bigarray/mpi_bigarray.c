#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
		//Prepare message and other parameters for getting process-related information
		char *message;
		int msg_len=20;
		int world_size;
		int world_rank;
		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Status status;
		message=malloc(msg_len*sizeof(char));

		// **********  INITIALIZING + PROCESS INFO RETRIEVE ***********
		// Initialize the MPI environment
		MPI_Init(NULL, NULL);

		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

		// Get the name of the processor
		MPI_Get_processor_name(processor_name, &name_len);

		// ********** OPERATIONS START FROM HERE **********
		if (world_rank==0) 
		{
			strcpy(message, "Hello");
			MPI_Send(message, msg_len, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
		} else {
			MPI_Recv(message, msg_len, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
			printf("%s received: %s by process %d out of %d processes\n", processor_name, message, world_rank, world_size);
		}

		// Finalize the MPI environment
		MPI_Finalize();
		free(message);
		return 0;
}
