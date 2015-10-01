#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

char * getTimestamp(){
        static char buff[20];
        struct timespec tps;
        clock_gettime(CLOCK_REALTIME, &tps);
        unsigned long now = tps.tv_sec * 1000 + tps.tv_nsec / 1000000;
        sprintf(buff, "%lu", now);
        return buff;
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
		
		printf("[%s]: Process #%d: passed barrier.", getTimestamp(), world_rank);

}
