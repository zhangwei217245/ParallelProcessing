#include <stdio.h>
#include <unistd.h>
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
char * getTimeString(){
		static char buff[20];
        struct timespec tps;
        clock_gettime(CLOCK_REALTIME, &tps);
        unsigned long now_milli = tps.tv_nsec / 1000000;
        time_t now = tps.tv_sec;
		//time_t now = time(NULL);
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
		sprintf(buff, "%s:%03lu", buff, now_milli);
		return buff;
}
int my_barrier(MPI_Comm comm){

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


		printf("[%s]: Process #%d starts.\n", getTimeString(), world_rank);
		// Each process will carry out some calculation or operations, which takes different times.
		if (world_rank % 2 == 0){ 
				sleep(3);
		} else {
				sleep(1);
		}
		
		printf("[%s]: Process #%d is entering barrier.\n", getTimeString(), world_rank);
		// Call the barrier function here
		//my_barrier(MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		// print the message after the barrier, by each process.
		printf("[%s]: Process #%d: passed barrier.\n", getTimeString(), world_rank);

		// Finalize the MPI environment
		MPI_Finalize();
		return 0;
}
