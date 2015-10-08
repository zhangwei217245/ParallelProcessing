#include "my_barrier.h"


int my_barrier(MPI_Comm comm){
	return butterfly_barrier_plain(comm);
}
