#include <string.h>
#include "util.h"
#include "my_barrier.h"

/**
 * return 1 for MPI_Barrier, and 0 for my_barrier;
 * return -1 for unrecognizable value of argument; 
**/
int determineBarrierType(int argc, char *argv[])
{
		char bar_type[]="-bar";
		int i=0;
		for ( ; i < argc; i++)
		{
				if (strncmp(bar_type, argv[i], 4) == 0 && (i+1) < argc)
				{
						
						if (strncmp(argv[i+1], "mpi", 3)==0) {
								return 1;
						}
						if (strncmp(argv[i+1], "my", 2)==0) {
								return 0;
						}
				}
		}
		return -1;
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
		if (determineBarrierType(argc, argv) <= 0){
				my_barrier(MPI_COMM_WORLD);
		}else {
				MPI_Barrier(MPI_COMM_WORLD);
		}
		// print the message after the barrier, by each process.
		printf("[%s]: Process #%d: PASSED barrier.\n", getTimeString(), world_rank);

		// Finalize the MPI environment
		MPI_Finalize();
		return 0;
}
