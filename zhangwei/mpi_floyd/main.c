/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 */
#include <stdio.h>
#include "util.h"
#include "floyd.h"
#include <mpi.h>

void parseArgs(int argc, char* argv[], int *n){
		char msize[]="-msize";
		// Read args.
		int i;
		for ( i = 0; i < argc; i++){
				if (strncmp(msize, argv[i], 6) == 0 && (++i) < argc){
						if (argv[i] != NULL){
								*n = atoi(argv[i]);
						}
				}
		}
}
int sanityCheck(int world_rank, int world_size, int n){
		double sqrt_p = sqrt((double)world_size);
		double flor_sqrt_p = floor(sqrt_p);
		if (sqrt_p != flor_sqrt_p){
				if (world_rank == 0){
						printf("The number of processes must be a perfect square number!\n");
				}
				return -1;
		}
		if (n % ((int)sqrt_p) != 0) {
				if (world_rank == 0){
						printf("The size of the matrix is not divisible by the square root of number of processes.\n");
				}
				return -1;
		}
		return 1;
}
int main(int argc, char * argv[]){
		int world_size, world_rank;
		// **********  INITIALIZING + PROCESS INFO RETRIEVE ***********
		// Initialize the MPI environment
		MPI_Init(&argc, &argv);

		// Get the number of processes and the rank of the processor
		getSizeAndRank(&world_size, &world_rank, MPI_COMM_WORLD);
		// Parse the argument
		int n=2;
		parseArgs(argc, argv, &n);	
		// Sanity Check, ensure that the parameters are valid.
		if (sanityCheck(world_rank, world_size, n) < 0){
				MPI_Finalize();
				return 0;
		}
		// Generating the matrix and print it out.
		int **edge = NULL;
		if (world_rank == 0){
				edge = generateMatrix(n);
				printf(":::::::::::::::: Original Matrix :::::::::::::::::\n");
				printMatrix(edge, n);
				printf("::::::::::::::::::::::::::::::::::::::::::::::::::\n");
		}
		// do the parallel floyd algorithm
		floyd(n, edge);
		// show the result after parallel floyd algorithm
		if ( world_rank == 0){
				printf(":::::::::::::::::: Final Matrix ::::::::::::::::::\n");
				printMatrix(edge, n);
				printf("::::::::::::::::::::::::::::::::::::::::::::::::::\n");
		}
		// Finalize the MPI environment
		MPI_Finalize();
		return 0;
}
