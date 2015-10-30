/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */
#include "floyd.h"

int ** floyd(int n, int **original){

		int k = 0, i = 0, j = 0;

		// Get the world_size and world_rank
		int world_size, world_rank;
		getSizeAndRank(&world_size, &world_rank);

		// Calculating sqrt_p;
		int sqrt_p = (int)sqrt((double)world_size);

		// Calculating the size of submatrix,namely the grid size;
		int grid_size = world_size / sqrt_p;

		// Initialize buffer for every process to receive message
		int **buf;
		buf=(int **) calloc(sizeof(int *), grid_size);
		for (i = 0; i < grid_size; i++){
				buf[i] = (int *) calloc (sizeof(int),grid_size);
		}
		// Get the group for all processes
		MPI_Group world;
		MPI_Comm_group(MPI_COMM_WORLD, &world);
		// generating sqrt(p) of horizontal communicators
		MPI_Comm *horz_comms = (MPI_Comm *) malloc(sqrt_p * sizeof(MPI_Comm));
		for (i = 0; i < sqrt_p; i++) { 
				int *horz_ranks = (int *)calloc(sizeof(int),sqrt_p);
				for (j = 0; j < sqrt_p; j++){
						horz_ranks[j] = sqrt_p * i + j;
				}
				MPI_Group horz_grp;
				MPI_Group_incl(world, sqrt_p, horz_ranks, &horz_grp);
				MPI_Comm_create(MPI_COMM_WORLD, horz_grp, &horz_comms[i]);
		}
		//
		// generating sqrt(p) of vertical communicators
		MPI_Comm *vert_comms = (MPI_Comm *) malloc(sqrt_p * sizeof(MPI_Comm));
		for (j = 0; j < sqrt_p; j++){
				int *vert_ranks = (int *) calloc(sizeof(int), sqrt_p);
				for (i = 0; i < sqrt_p; i++){
						vert_ranks[i] = sqrt_p * i + j;
				}
				MPI_Group vert_grp;
				MPI_Group_incl(world, sqrt_p, vert_ranks, &vert_grp);
				MPI_Comm_create(MPI_COMM_WORLD, vert_grp, &vert_comms[j]);
		}
		//
		// process 0 distributes the data among P all processes
		if (world_rank == 0){
				for (k = 0; k < world_size; k++){
						int R = world_size / sqrt_p;
						int C = world_size % sqrt_p;
						for (i = R * grid_size; i < (R+1) * grid_size; i++){
								int start_pos = C * grid_size;
								MPI_Send(&original[i][start_pos], grid_size, MPI_INT, k, 0, MPI_COMM_WORLD);
						}
				}
		}
		// every process is receiving the data chunk from process 0
		MPI_Status status;
		for (i = 0; i < grid_size; i++){
				MPI_Recv(&buf[i], grid_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		}
		//
		// each process enters into the while loop, run the loop for n times
		k = 0;
		while (k < n){
				int *horz_buff = (int *) calloc(sizeof(int),grid_size);
				int *vert_buff = (int *) calloc(sizeof(int),grid_size);

				// The index for column and row which contains the sender processes.
				int si = k/grid_size;
				// find the senders in the row, and they will broadcast the message to their sibling processes vertically.
				for ( i = 0; i < sqrt_p; i++){
						int p_row = si * sqrt_p + i;
						int vert_comm_idx = p_row % sqrt_p;
						// if the current process is the sender, we fill the buffer. otherwise, we do nothing with the buffer.
						if (world_rank == p_row){
								for (j = 0; j < grid_size; j++){
										horz_buff[j] = buf[k%grid_size][j];
								}
						}
						// for all processes in communicator[vert_comm_idx], call the bcast function.
						if (world_rank % sqrt_p == vert_comm_idx){
								MPI_Bcast(&horz_buff, grid_size, MPI_INT, p_row, vert_comms[vert_comm_idx]);
						}
				}
				// find the senders in the column, and they will broadcast the message to their sibiling processes horizontally. 
				for (i = 0; i < sqrt_p; i++){
						int p_col = si + i * sqrt_p;
						int horz_comm_idx = p_col / sqrt_p;
						// if the current process is the sender, we fill the buffer, otherwise, we do nothing with the buffer.
						if (world_rank == p_col){
								for (j = 0 ; j < grid_size ; j++){
										vert_buff[j] = buf[j][k%grid_size];
								}
						}
						// for all processes in communicator[horz_comm_idx], call the bcast function.
						if (world_rank / sqrt_p == horz_comm_idx) {
								MPI_Bcast(&vert_buff, grid_size, MPI_INT, p_col, horz_comms[horz_comm_idx]);
						}
				}
				// Calculate the minimum value and update the element i and j
				for ( i = 0 ; i < grid_size ; i++){
						for (j = 0; j < grid_size; j++){
								buf[i][j] = min(buf[i][j], safesum(vert_buff[i], horz_buff[j]));
						}
				}
		}

		// collect the data from all processes and return it.
		// every process will send the data in sub matrix row by row.
		for (i = 0; i < grid_size; i++){
				MPI_Send(&buf[i], grid_size, MPI_INT, 0, i, MPI_COMM_WORLD);
		}		
		// the master process will receive the data row by row from all the processes
		// and put them in to the right place of the original matrix.

		if (world_rank == 0){
		// receiving the data from every processes row by row and save it to the original matrix 
				MPI_Status status;
				for (i = 0; i < grid_size; i++){
						for (k = 0; k < world_size; k++){
						
								int orig_row = k / sqrt_p * grid_size + i;
								int orig_col = k % sqrt_p * grid_size;

								MPI_Recv(&original[orig_row][orig_col], grid_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

						}
				}
		}
		

		// before return, free all groups
		// free all communicators
		// free all buffers
		return original;

}