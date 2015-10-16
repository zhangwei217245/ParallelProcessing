/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */
#include "dijkstra.h"
#include <mpi.h>



void getSizeAndRank(int *world_size, int *world_rank){
		
		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, world_size);
		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, world_rank);
}

int chooseVertex(int *dist, int n, int *found){
		int i, tmp, leastPosition=0;
		int least = INFTY;

		for (i = 0; i < n; i++){
				tmp = dist[i];
				if ((!found[i]) && (tmp < least)){
						least = tmp;
						leastPosition = i;
				}
		}
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		return leastPosition;
}
void updateDist(int *dist, int n, int *found, int *row){
		int infinity = INFTY;
		int i;
		int distJ = row[n];
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		for (i = 0; i < n; i++){
				if (!(found[i])){
						int distplus = (row[i] == infinity)? infinity: distJ + row[i];
						dist[i] = min(dist[i], distplus);
				}
		}	
}
/**
 * This function will be excuted only by any process, only process 0 passes the valid parameter.
 * n always equals to world_size * k, 
 * where k is a constant specified by user in the client function, 
 * e.g. main function.
 */
void dijkstra(int SOURCE, int n, int **edge, int *dist){
		int world_size, world_rank;
		getSizeAndRank(&world_size, &world_rank);
		int chunkSize = n / world_size;
		int *msgBuf = (int *)calloc(sizeof(int), chunkSize*2);
		int *buff = (int *) calloc(sizeof(int), (chunkSize*3+1));
		MPI_Status status;
		MPI_Request requestNull;
		if (world_rank == 0) {
				// Phase 1: Rank 0 prepares the data for all the other processes.
				int i, j, count, *found;
				found = (int*) calloc (n, sizeof(int));
				for (i = 0; i < n; i++){
						found[i]=0;
						dist[i]=edge[SOURCE][i];
				}

				found[SOURCE] = 1;
				count = 1;
				// Phase 2: take the shorted one among all the paths from SOURCE to all the other nodes.
				while ( count < n ){
						// Divide messages, send them to all the processes asynchronously.
						for (i = 0; i < world_size; i++){
								int start = chunkSize * i;
								int *msg = appendIntArray(&dist[start], chunkSize, &found[start], chunkSize);
								if ( i == 0) {
										// Message send to rank 0 will immediately received by rank 0 itself.	
										// Basically copy the part of data into *_dist of rank 0;
										MPI_Sendrecv(msg, chunkSize*2, MPI_INT, i, count, msgBuf, chunkSize*2, MPI_INT, i, count, MPI_COMM_WORLD, &status);
								} else {
										// Sending messages to all other processes. 
										// Recv in other processes, then they call chooseVertex.
										MPI_Send(msg, chunkSize*2, MPI_INT, i, count, MPI_COMM_WORLD);	
								}
								free(msg);
						}
						// for rank 0, calculate the partial result for j locally.
						j = chooseVertex(msgBuf, chunkSize, &msgBuf[chunkSize]);
						// for other processes, use MPI_Recv to receive their corresponding partial results.
						// and then, a global J (mininum) will be determined.
						for ( i = 1; i < world_size; i++){
								int j_tmp;
								// Message sent from other processes after they call chooseVertex.
								MPI_Recv(&j_tmp, 1, MPI_INT, i, count, MPI_COMM_WORLD, &status);
								// determining of global J (mininum).
								j_tmp = chunkSize * i + j_tmp;
								if (dist[j_tmp] < dist[j]) {
										j = j_tmp;
								}
						}
						// The for-loop above is kind of barrier in this way.
						// Done with selecting the vertex which has the minimum distance with SOURCE.
						found[j] = 1;
						count++;
						// Phase 3: Updating the dist array in parallel.
						// Send n/world_size data items to different process, let them update their corresponding data area. 
						for (i = 0; i < world_size; i++){
								int start = chunkSize * i;
								int *msg = appendIntArray(&dist[start], chunkSize, &found[start], chunkSize);
								msg = appendIntArray(msg, chunkSize*2, &edge[j][start], chunkSize);
								msg = appendIntArray(msg, chunkSize*3, &dist[j], 1);
								if ( i == 0) {
										// Message send to rank 0 will immediately received by rank 0 itself.	
										// Basically copy the part of data into *_dist of rank 0;
										MPI_Sendrecv(msg, (chunkSize*3+1), MPI_INT, i, count, buff, (chunkSize*3+1), MPI_INT, i, count, MPI_COMM_WORLD, &status);
								} else {
										// Sending messages to all other processes. 
										// To be Received in other processes, then they call updateDist
										MPI_Send(msg, (chunkSize*3+1), MPI_INT, i, count, MPI_COMM_WORLD);	
								}
								free(msg);
						}
						updateDist(buff, chunkSize, &buff[chunkSize], &buff[chunkSize*2]);
						memcpy(dist, buff, chunkSize * sizeof(int));
						for ( i = 1; i < world_size; i++){
								int start = chunkSize * i;
								// Message Sent from other processes after they call updateDist
								MPI_Recv(&dist[start], chunkSize, MPI_INT, i, count, MPI_COMM_WORLD, &status);
						}
				}
		} else {
				int cnt = 1;
				while ( cnt < n ){
						// Receive the message, choose the Vertex, send back the result.
						MPI_Recv(msgBuf, chunkSize*2, MPI_INT, 0, cnt, MPI_COMM_WORLD, &status);
						int j_tmp = chooseVertex(msgBuf, chunkSize, &msgBuf[chunkSize]);
						MPI_Send(&j_tmp,1, MPI_INT, 0, cnt, MPI_COMM_WORLD);
						cnt++;
						// Receive the message, update the dist, send back the result.
						MPI_Recv(buff, (chunkSize*3+1), MPI_INT, 0, cnt, MPI_COMM_WORLD, &status);
						updateDist(buff, chunkSize, &buff[chunkSize], &buff[chunkSize*2]);
						MPI_Send(buff, chunkSize, MPI_INT, 0, cnt, MPI_COMM_WORLD);
				}
		}
		free(msgBuf);
		free(buff);
}
