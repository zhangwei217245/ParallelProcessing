#include "butterfly_barrier.h"
#include <math.h>
#include "util.h"

int butterfly_barrier(MPI_Comm comm){
		int world_size, world_rank;
		int i = 0, x = 0;
		
		MPI_Status status;
		
		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);	

		int last_mid = world_size;
		
		int totalSteps = (int)lg((double) world_size);
		for (i = 1; i <= totalSteps; i++){
				int pow_2 = (int)pow((double)2, (double)i);
				int mid = world_size / pow_2;
				int addr = 0;
				if (world_rank % last_mid < mid) {
						addr = world_rank + mid;
				} else {
						addr = world_rank - mid;
				}
				int x;
				MPI_Send(&x, 1, MPI_INT, addr, 0, MPI_COMM_WORLD);
				MPI_Recv(&x, 1, MPI_INT, addr, 0, MPI_COMM_WORLD, &status);
				last_mid = mid;
		}
		printf("[%s]: Butterfly Barrier done for #%d.\n", getTimeString(), world_rank);
		return 0;
}
