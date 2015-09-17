#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

double get_avg(int len, int *arr){
	int sum = 0;
	int i;
	for (i = 0; i < 1024; i++) {
		sum = sum + arr[i];
	}
	return (double)sum/1024;
}
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
	time_t now = time(NULL);
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));	
	return buff;
}

int sanityCheck(int world_size)
{
	int allowed_size[6] = {1,2,3,4,6,12};
	int size_found=-1;
	int idx;
	for (idx = 0; idx < 6; idx++) {
		if (allowed_size[idx]==world_size){
			size_found=idx;
		}
	}
	return size_found;
}

int parseArguments(int argc, char *argv[], int *send_mode, int *fill_mode, int *wait_mode)
{
	char a_wait[]="-wait";
	char a_send[]="-send";
	char a_fill[]="-fill";
	int i=0;
	for ( ; i < argc; i++)
	{
		if (strncmp(a_wait, argv[i], 5) == 0 && (i+1) < argc)
		{
			*wait_mode = atoi(argv[i+1]);
			if (*wait_mode < 0 || *wait_mode >1 ){
				return -1;
			}	
		}
		if (strncmp(a_send, argv[i], 5) == 0 && (i+1) < argc)
		{
			*send_mode = atoi(argv[i+1]);
			if (*send_mode < 0 || *send_mode > 1){
				return -1;
			}
		}
		if (strncmp(a_fill, argv[i], 5) == 0 && (i+1) < argc)
		{
			*fill_mode = atoi(argv[i+1]);
			if (*fill_mode < 0 || *fill_mode > 2){
				return -1;
			}
		}
	}
	return 0;
}
void generatingWhileSending(int row_count_per_process, int world_size, int world_rank, MPI_Request * requestNull, int send_mode, int fill_mode)
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
		// fill row with random numbers.
		for (col=0; col<1024; col++) {
			int cell_content = rand() % (row+1);
			if (fill_mode == 1){
				cell_content = rand();
			}
			if (fill_mode == 2){
				cell_content = row;
			}
			array[row][col] = cell_content; 
		}
		if (send_mode == 0) {
			MPI_Isend(&array[row], 1024, MPI_INT, prank, row, MPI_COMM_WORLD, requestNull);
			printf("%s : row %d sent to prank #%d by rank #%d right after it is generated.\n", getTimestamp(), row, prank, world_rank);
		}
		row++;
		if (row % row_count_per_process == 0) {
			if (send_mode != 0) {
				int s_back = row - row_count_per_process;
				for ( ; s_back < row; s_back++) {
					MPI_Isend(&array[s_back], 1024, MPI_INT, prank, s_back, MPI_COMM_WORLD, requestNull);
					printf("%s : row %d sent to prank #%d by rank #%d in batch mode.\n", getTimestamp(), s_back, prank, world_rank);
	
				}
			}
			prank++;
		}
	}
}

void receiving(int row_count_per_process, int world_rank, int msgbuf[][1024], MPI_Request *requestList)
{
	int k;
	for (k = 0; k < row_count_per_process; k++) {
		int row_num = world_rank * row_count_per_process + k;
		MPI_Irecv(&msgbuf[k], 1024, MPI_INT, 0, row_num, MPI_COMM_WORLD, &(requestList[k]));
		printf("%s : MPI_Irecv is called for receiving row #%d by rank #%d\n", getTimestamp(), row_num, world_rank);
	}		
}

void calculateWithMPI_Test(int row_count_per_process, MPI_Request *requestList, MPI_Status * status, int msgbuf[][1024], int world_rank, char * processor_name){
	
	// ****** Calculating *******
	int calculated = 0;
	while ( calculated < row_count_per_process) 
	{
		int flag = 0;
		int index;
		for (index = 0; index < row_count_per_process; index++) {
			if (requestList[index] == NULL)
			{
				continue;
			}
			MPI_Test(&(requestList[index]), &flag, status);
			if ( flag > 0 ) {
				requestList[index] = NULL;
				// Calculating Average
				double avg = get_avg(1024, msgbuf[index]);
				int row_num = world_rank * row_count_per_process + index;
				printf("%s : Process #%d (%s) : average of row #%d = %f (MPI_Test)\n", getTimestamp(), world_rank, processor_name, row_num, avg);
				calculated++;
			}
		}
	}
}
void calculateWithMPI_Waitany(int row_count_per_process, MPI_Request *requestList, MPI_Status *status, int msgbuf[][1024], int world_rank, char *processor_name){

	// ****** Calculating *******
	int calculated = 0;
	while ( calculated < row_count_per_process) 
	{
		int index;
		MPI_Waitany(row_count_per_process, requestList, &index, status);
		// Calculating Average
		double avg = get_avg(1024, msgbuf[index]);
		int row_num = world_rank * row_count_per_process + index;
		printf("%s : Process #%d (%s) : average of row #%d = %f (MPI_Waitany)\n", getTimestamp(), world_rank, processor_name, row_num, avg);
		calculated++;
	}
}

void print_help(int world_rank){
	if (world_rank == 0) {
		printf("Arguments:\n");
		printf(" -wait : \n");
		printf("\t0: calculate with MPI_Test; \n");
		printf("\t1: calculate with MPI_Waitany.\n");
		printf(" -send : \n");
		printf("\t0: send each row right after it is generated; \n");
		printf("\t1: send rows for particular process after they are generated totally.\n");
		printf(" -fill : \n");
		printf("\tFill the array with: \n");
		printf("\t0 - random integer mod row_count+1; \n");
		printf("\t1 - random integer; \n");
		printf("\t2 - row_count.\n");
	}
}

int main (int argc, char *argv[])
{
	//Prepare message and other parameters for getting process-related information
	int wait_mode=0; //0: calculate with MPI_Test; 1: calculate with MPI_Waitany
	int send_mode=0; //0: send each row right after it is generated; 1: send rows for particular process after they are generated totally.
	int fill_mode=0; //Fill the array with: 0 - random integer mod row_count+1; 1 - random integer; 2 - row_count. 	

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
	if (sanityCheck(world_size) < 0) {
		if (world_rank == 0) {
			printf("The number of processes must be 1/2/3/4/6/12. \n");
		}
		MPI_Finalize();
		return 0;
	}

	//Parse arguments to decide wait mode and send mode.
	if (parseArguments(argc, argv, &send_mode, &fill_mode, &wait_mode) < 0)
	{
		print_help(world_rank);
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
	receiving(row_count_per_process, world_rank, msgbuf, requestList);

	// Once receiving channel is working, generating array while sending it.
	if (world_rank==0) 
	{
		//generating while sending.
		generatingWhileSending(row_count_per_process, world_size, world_rank, &requestNull, send_mode, fill_mode);	
	} 

	// ****** Calculating *******
	if (wait_mode == 0) {
		calculateWithMPI_Test(row_count_per_process, requestList, &status, msgbuf, world_rank, processor_name);	
	} else {
		calculateWithMPI_Waitany(row_count_per_process, requestList, &status, msgbuf, world_rank, processor_name);
	}
	// Finalize the MPI environment
	MPI_Finalize();
	free(requestList);
	return 0;
}

