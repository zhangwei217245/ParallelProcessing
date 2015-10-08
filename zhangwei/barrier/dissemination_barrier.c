#include "dissemination_barrier.h"
#include <math.h>
#include "util.h"

int dissemination_barrier(MPI_Comm comm){
		int world_size, world_rank;
		int i = 0;
		
		MPI_Status status;
		
		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);	
		
		int totalSteps = ceiling(lg((double) world_size));
		for (i = 0; i < totalSteps; i++){
				int pow_2 = (int)pow((double)2, (double)i);
				int dst = (world_rank + pow_2) % world_size;
				int src = world_size - ((world_rank - pow_2) % world_size);
				MPI_Send(&i, 1, MPI_INT, dst, 0, MPI_COMM_WORLD);
				MPI_Recv(&i, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &status);
		}
		printf("[%s]: Dissemination Barrier done for #%d.\n", getTimeString(), world_rank);
		return 0;
}
