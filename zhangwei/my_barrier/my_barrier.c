//Wei Zhang, Ali Nosrati

#include "mpi.h"
#include <string.h>
#include "util.h"

int my_barrier(MPI_Comm comm) {
		int world_size, world_rank;
		int i = 0, x = 0;

		MPI_Status status;

		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

		int totalSteps = ceiling(lg((double) world_size));
		for (i = 0; i < totalSteps; i++){
				int pow_2 = (int)pow((double)2, (double)i);
				int dst = (world_rank + pow_2) % world_size;
				int src = (world_rank - pow_2 + world_size) % world_size;
				printf("[%s] %d -> %d -> %d\n", getTimeString(),src, world_rank, dst);
				MPI_Send(&i, 1, MPI_INT, dst, 0, MPI_COMM_WORLD);
				MPI_Recv(&x, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &status);
		}
		printf("[%s]: Dissemination Barrier done for #%d.\n", getTimeString(), world_rank);
		return 0;
}
int main(int argc, char *argv[])
{	

		int world_size;
		int world_rank;

		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;

		// **********  INITIALIZING + PROCESS INFO RETRIEVE ***********
		// Initialize the MPI environment
		MPI_Init(&argc, &argv);

		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

		// Get the name of the processor
		MPI_Get_processor_name(processor_name, &name_len);


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

		// Finalize the MPI environment
		MPI_Finalize();
		return 0;
}
