#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
		//Prepare message and other parameters for getting process-related information
		char message[20];
		int world_size;
		int world_rank;
		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;

		// Initialize the MPI environment
		MPI_Init(NULL, NULL);

		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

		// Get the name of the processor
		MPI_Get_processor_name(processor_name, &name_len);

		// Print off a hello world message
		printf("hello world from processor %s, rank %d"
						" out of %d processors\n",
						processor_name, world_rank, world_size);

		// Finalize the MPI environment
		MPI_Finalize();

		return 0;
}
