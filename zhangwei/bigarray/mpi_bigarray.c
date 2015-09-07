#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
		//Prepare message and other parameters for getting process-related information
		int world_size;
		int world_rank;
		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Status status;
		MPI_Request *requestList, requestNull;

		// **********  INITIALIZING + PROCESS INFO RETRIEVE ***********
		// Initialize the MPI environment
		MPI_Init(&argc, &argv);

		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		
		// Get the name of the processor
		MPI_Get_processor_name(processor_name, &name_len);
		
		//Sanity Check
		int allowed_size[6] = {1,2,3,4,6,12};
		int size_found=0;
		int idx;
		for (idx = 0; idx < 6; idx++) {
			if (allowed_size[idx]==world_size){
				size_found=1;
			}
		}
		if (size_found!=1) {
			if (world_rank == 0) {
				printf("The number of processes must be 1/2/3/4/6/12. \n");
			}
			MPI_Finalize();
			return 0;
		}
		// ********** OPERATIONS START FROM HERE **********

		int row_count_per_process = 1200/world_size;

		// ********** IRecv Start First ****************
		// ********** Overlap with both sending and generating, in addition, calculating **********
		
		// Prepare RequestList
		requestList = (MPI_Request*) malloc(row_count_per_process * sizeof(MPI_Request));
		// Prepare message buffer;
		int msgbuf[row_count_per_process][1024];
		
		// Start Receiving
		int k;
		for (k = 0; k < row_count_per_process; k++) {
			int row_num = world_rank * row_count_per_process + k;
			MPI_Irecv(&msgbuf[k], 1024, MPI_INT, 0, row_num, MPI_COMM_WORLD, &(requestList[k]));
			//printf("row %d received by rank %d\n", row_num, world_rank);
		}		
		// Once receiving channel is working, generating array while sending it.
		if (world_rank==0) 
		{
			int array[1200][1024];
			// ******** Generating the Array. **********
			// ******** Generating each row with random numbers *********
			int row=0;
			int col=0;
			int prank=0;
			/* Intializes random number generator */
			time_t t;
   			srand((unsigned) time(&t));
			
			// ******** Overlapping 1 here, Generating while Sending.
			while(prank < world_size)
			{
				// fill row with random numbers within 0-99.
				for (col=0; col<1024; col++) {
					array[row][col] = rand();
				}
				MPI_Isend(&array[row], 1024, MPI_INT, prank, row, MPI_COMM_WORLD, &requestNull);	
				//printf("row %d sent to prank %d\n", row, prank);
				row++;
				if (row % row_count_per_process == 0) {
					prank++;
				}
				
			}
			
		} 

		// ****** Calculating *******
		int calculated = 0;
		while ( calculated < row_count_per_process) 
		{
			int index;
			MPI_Waitany(row_count_per_process, requestList, &index, &status);
			// Calculating Average
			int sum = 0;
			int i;
			for (i = 0; i < 1024; i++) {
				sum = sum + msgbuf[index][i];
			}
			int row_num = world_rank * row_count_per_process + index;
			double avg = (double)sum/1024;
			printf("Process #%d (%s) : average of row #%d = %f\n", world_rank, processor_name, row_num, avg);
			calculated++;
		}
		
	
		// Finalize the MPI environment
		MPI_Finalize();
		free(requestList);
		return 0;
}
