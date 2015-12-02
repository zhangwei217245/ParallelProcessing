/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */
#include "util.h"
#include <math.h>


void printMatrix(int **edge, int n){
		int i, j, infinity = INFTY;
		for(i = 0; i < n; i++){
				for (j = 0; j < n; j++){
						if ( infinity == edge[i][j] ){
								printf("INFTY\t");
						} else {
								printf("%d\t", edge[i][j]);
						}
				}
				printf("\n");
		}
		printf("\n");
}
int **generateMatrix(int n){
		int i, j;
		//Initializing Matrix
		int **edge;
		edge=(int **) calloc(sizeof(int *), n);
		for (i = 0; i < n; i++){
				edge[i] = (int *) calloc (sizeof(int),n);
		}
		//Fill Matrix with Random number with a few of them being INFTY, keep the element on diagonal as zero,
		srand((unsigned int) time(NULL));
		for (i = 0; i < n; i++){
				int infPos = rand() % n;//randomly insert a INFTY in each row.
				if ( infPos == 0 || infPos == (n-1)){
						infPos = -1;//The first and last element shouldn't be INFTY
				}
				for (j = 0; j < n; j++){
						if (i == j){
								edge[i][j] = 0;
						} else if (j == infPos){
								edge[i][j] = INFTY;
						} else {
								edge[i][j] = rand() % 20;
						}
				}		
		}
		return edge;
}
int safesum(int s1, int s2 ){
		int infinity = INFTY;
		int sum = (s1 == infinity || s2 == infinity) ? infinity : (s1+s2);
		return sum;
}
double lg(double n){
		return log(n)/log(2);
}
void getSizeAndRank(int *size, int *rank, MPI_Comm comm){

		// Get the number of processes
		MPI_Comm_size(comm, size);
		// Get the rank of the processor
		MPI_Comm_rank(comm, rank);
}
int ceiling(double n){
		return (int)ceil(n);
}
double flooor(double n){
		return floor(n);
}
int min(int i1, int i2){
		if (i2 < i1){
				return i2;
		} else {
				return i1;
		}
}
void printArray(int *arr, int n){
		int i;
		for (i = 0; i < n; i ++){
				printf("%d ", arr[i]);
		}
		printf("\n\n");
}
