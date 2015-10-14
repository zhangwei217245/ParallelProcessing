/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */

#include <stdio.h>
#include "util.h"
#include "dijkstra.h"

int main(int argc, char * argv[]){
		int n = 5, i;
		int *dist = (int *) calloc(sizeof(int), n);
		int **edge = generateMatrix(n);
		printMatrix(edge, n);
		dijkstra(3 , n, edge, dist);

		for (i = 0; i < n; i++){
				printf("%d ", dist[i]);
		}
		printf("\n");

}
