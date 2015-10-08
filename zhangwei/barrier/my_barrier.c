#include "butterfly_barrier.h"
#include "tree_barrier.h"
#include "dissemination_barrier.h"
#include "my_barrier.h"
#include "util.h"

int my_barrier(MPI_Comm comm){
		int world_size;
		int i = 0;

		MPI_Status status;

		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		
		double log_2P = lg(world_size);
		double floor = flooor(log_2P);
		double zero = 0.0000000;
		
		if (log_2P - floor != zero) {
				return dissemination_barrier(comm);
		} else if (world_size <= 8) {
				return tree_barrier(comm);
		} else {
				return butterfly_barrier(comm);
		}
}
