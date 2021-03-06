/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */
#include "omp.h"
#include "floyd.h"

int ** floyd(int n, int **original){

		int k = 0, i = 0, j = 0;

		// Get the world_size and world_rank
		int world_size, world_rank;
		getSizeAndRank(&world_size, &world_rank, MPI_COMM_WORLD);

		// Calculating sqrt_p;
		int sqrt_p = (int)sqrt((double)world_size);

		// Calculating the size of submatrix,namely the grid size;
		int grid_size = n / sqrt_p;

		// Initialize buffer for every process to receive message
		int **buf;
		buf=(int **) malloc(sizeof(int *) * grid_size);
		for (i = 0; i < grid_size; i++){
				buf[i] = (int *) calloc (sizeof(int),grid_size);
		}
		// Create row-based communicator and column-based communicator.
		int r = world_rank / sqrt_p;
		int c = world_rank % sqrt_p;
		MPI_Comm row_comm, col_comm;
		MPI_Comm_split(MPI_COMM_WORLD, r, world_rank, &row_comm);
		MPI_Comm_split(MPI_COMM_WORLD, c, world_rank, &col_comm);
		int row_rank, row_size, col_rank, col_size;
		getSizeAndRank(&row_size,&row_rank, row_comm);
		getSizeAndRank(&col_size,&col_rank, col_comm);
		
		k = 0;
		int *horz_buff = (int *) calloc(sizeof(int),grid_size);
		int *vert_buff = (int *) calloc(sizeof(int),grid_size);

		omp_set_num_threads(grid_size);
		#pragma omp parallel shared(horz_buff, vert_buff, original, buf)	
		{
				// process 0 distributes the data among P all processes
				if (world_rank == 0){
						#pragma omp barrier
						//#pragma omp master
						#pragma omp for private(i,k) 
						for (i = 0; i < grid_size; i++){
								for (k = 0; k < world_size; k++){
										int R = k / sqrt_p * grid_size + i;
										int C = k % sqrt_p * grid_size;
										#pragma omp critical
										MPI_Send(&original[R][C], grid_size, MPI_INT, k, i, MPI_COMM_WORLD);
								}
						}
						#pragma omp barrier
				}
				// every process is receiving the data chunk from process 0
				MPI_Status status;
				#pragma omp barrier
				//#pragma omp master
				#pragma omp for private(i)
				for (i = 0; i < grid_size; i++){
						#pragma omp critical
						MPI_Recv(&buf[i][0], grid_size, MPI_INT, 0, i, MPI_COMM_WORLD, &status);
				}
				#pragma omp barrier
		}
		//
		// each process enters into the while loop, run the loop for n times
		while (k < n){
				printf("--======== rank_%d : k = %d =========--\n", world_rank, k);
				printMatrix(buf, grid_size);
				// The index for column and row which contains the sender processes.
				int si = k / grid_size;
				// Broadcast the kth row vertically:
				if (col_rank == si) {
						for (j = 0; j < grid_size; j++){
								horz_buff[j] = buf[k % grid_size][j];
						}
				}
				//#pragma omp critical
				MPI_Bcast(horz_buff, grid_size, MPI_INT, si, col_comm);


				// Broadcast the kth column horizontally:
				if (row_rank == si) {
						for (j = 0; j < grid_size; j++){
								vert_buff[j] = buf[j][k % grid_size];
						}
				}
				//#pragma omp critical
				MPI_Bcast(vert_buff, grid_size, MPI_INT, si, row_comm);

				// Calculate the minimum value and update the element i and j
				double start, end;

				omp_set_num_threads(grid_size/(world_size/2));
				start = MPI_Wtime();
				#pragma omp parallel shared(buf, vert_buff, horz_buff)
				{

						//#pragma omp master
						#pragma omp for private(i,j) 
						for ( i = 0 ; i < grid_size ; i++){
								for (j = 0; j < grid_size; j++){
										buf[i][j] = min(buf[i][j], safesum(vert_buff[i], horz_buff[j]));
								}
						}
				}
				end = MPI_Wtime();
				printf("%.10f nano seconds used for doing the calculation on submatrix %d\n", end-start, world_rank);
				k++;
		}

		printf("--======== rank_%d : final sub_matrix =========--\n", world_rank);
		printMatrix(buf, grid_size);

		// collect the data from all processes and return it.
		// every process will send the data in sub matrix row by row.
		//
		omp_set_num_threads(grid_size);
		#pragma omp parallel shared (buf, original, grid_size)
		{
				#pragma omp barrier
				//#pragma omp master
				#pragma omp for private (i) 
				for (i = 0; i < grid_size; i++){
						#pragma omp critical
						MPI_Send(&buf[i][0], grid_size, MPI_INT, 0, i, MPI_COMM_WORLD);
				}
				#pragma omp barrier
		// the master process will receive the data row by row from all the processes
		// and put them in to the right place of the original matrix.

				if (world_rank == 0){
				// receiving the data from every processes row by row and save it to the original matrix 
						#pragma omp barrier
						//#pragma omp master
						#pragma omp for private(i,k)
						for (i = 0; i < grid_size; i++){
								for (k = 0; k < world_size; k++){
										int R = k / sqrt_p * grid_size + i;
										int C = k % sqrt_p * grid_size;
										MPI_Status recv_status;
										#pragma omp critical
										MPI_Recv(&original[R][C], grid_size, MPI_INT, k, i, MPI_COMM_WORLD, &recv_status);
								}
						}
						#pragma omp barrier
				}

		}
		

		// before returning, free all communicators
		MPI_Comm_free(&row_comm);
		MPI_Comm_free(&col_comm);
		// free all buffers
		free(vert_buff);
		free(horz_buff);
		free(buf[0]);
		free(buf);
		return original;

}
