#include "tree_barrier.h"
#include <math.h>
#include "util.h"


int tree_barrier(MPI_Comm comm){
		int world_size, world_rank;
		int i = 0, x = 0;
		
		MPI_Status status;
		
		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);	

		int totalSteps = (int)lg((double) world_size);
		// doing reduce 
		for (i = 1; i <= totalSteps; i++){
				int pow_2 = (int)pow((double)2, (double)i);
				int mid = world_size / pow_2;
				int window_size = mid * 2;
				int addr = 0;
				if (world_rank >= (world_size - window_size)) {
						if (world_rank % window_size < mid) {
								addr = world_rank + mid;
								//send message from low address to high address.
								MPI_Send(&x, 1, MPI_INT, addr, 0, MPI_COMM_WORLD);
						} else {
								addr = world_rank - mid;
								//waiting to receive message from low address to high address.
								MPI_Recv(&x, 1, MPI_INT, addr, 0, MPI_COMM_WORLD, &status);
						}
				}
		}
	
		
		// doing broadcast
		for (i = totalSteps; i >= 1; i--){
				int pow_2 = (int)pow((double)2, (double)i);
				int mid = world_size / pow_2;
				int window_size = mid * 2;
				int addr = 0;
				if (world_rank >= (world_size - window_size)){
						if (world_rank % window_size >= mid) {
								addr = world_rank - mid;
								//send message from high address to low address.
								MPI_Send(&x, 1, MPI_INT, addr, 0, MPI_COMM_WORLD);
						} else {
								addr = world_rank + mid;
								//receive message from high addresss.
								MPI_Recv(&x, 1, MPI_INT, addr, 0, MPI_COMM_WORLD, &status);
						}
				}
		}

		printf("[%s]: Tree Barrier done for #%d.\n", getTimeString(), world_rank);
		return 0;
}
