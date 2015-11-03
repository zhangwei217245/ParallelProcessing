#include <stdio.h>
#include "timeutil.h"
#include "util.h"
#include "mpi.h"
#include <omp.h>

int main(int argc, char *argv[]) {
		int numprocs, rank, namelen;
		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int iam = 0, np = 1;

		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Get_processor_name(processor_name, &namelen);
		
		int i, N=7;
		omp_set_num_threads(5);
		#pragma omp parallel
		{
				int ID = omp_get_thread_num();
				printf("%s ID=%d\n", getTimeString(), ID);
		}

		omp_set_num_threads(N);
		#pragma omp parallel default(shared) private(iam, np)
		{
				np = omp_get_num_threads();
				iam = omp_get_thread_num();
				printf("%s Hello from thread %d out of %d from process %d out of %d on %s\n",
				getTimeString(), iam, np, rank, numprocs, processor_name);
		}
		#pragma omp for
		for(i = 0; i < N; i++){
				printf("%d, %s Thread a one time. haha\n",i, getTimeString());
		}

		MPI_Finalize();
}
