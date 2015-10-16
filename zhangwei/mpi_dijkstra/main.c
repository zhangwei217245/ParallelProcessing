/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */

#include <stdio.h>
#include "util.h"
#include "dijkstra.h"
#include <mpi.h>


int main(int argc, char * argv[]){
		int world_size;
		int world_rank;

		// **********  INITIALIZING + PROCESS INFO RETRIEVE ***********
		// Initialize the MPI environment
		MPI_Init(&argc, &argv);

		// Get the number of processes
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		// Get the rank of the processor
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		// Two variables by user input
		int SOURCE=0, work_load=1;
		
		char source[]="-src";
		char load[]="-load";
		// Read args.
		int i = 0;
		for ( ; i < argc; i++){
				if (strncmp(source, argv[i], 4) == 0 && (++i) < argc){
						if (argv[i] != NULL){
								SOURCE = atoi(argv[i]);
						}
				}
				if (strncmp(load, argv[i], 5) == 0 && (++i) < argc){
						if (argv[i] != NULL){
								work_load = atoi(argv[i]);
						}
				}
				
		}
		

		int n = world_size * work_load; 
		if (SOURCE >= n){
				printf("Error SOURCE input, should be less than load*P, where P is number of processes.\n");
				SOURCE = 0;
		}
		int *dist = NULL;
		int **edge = NULL;
		if (world_rank == 0){
				dist = (int *) calloc(sizeof(int), n);
				edge = generateMatrix(n);
				printMatrix(edge, n);
		}

		dijkstra(SOURCE , n, edge, dist);
		
		if ( world_rank == 0){
				printf("rank 0, finish dijkstra\n");
				printf("dist:[ ");
				for (i = 0; i < n; i++){
						if ( dist[i] > 1000000) {
								printf("INFTY ");
						} else {
								printf("%d ", dist[i]);
						}
				}
				printf("]");
				printf("\n");
				free(dist);
				free(edge);
		}

		// Finalize the MPI environment
		MPI_Finalize();
		return 0;
}
