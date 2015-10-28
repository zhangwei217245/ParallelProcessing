#include "floyd.h"

int ** floyd(int n, int **original){
		int **tmp;
		int k = 0, i = 0, j = 0;
		tmp=(int **) calloc(sizeof(int *), n);
		for (i = 0; i < n; i++){
				tmp[i] = (int *) calloc (sizeof(int),n);
		}
	
		while (k < n){
				for ( i = 0 ; i < n ; i++){
						for (j = 0; j < n; j++){
								int infinity = INFTY;
								int sum = (original[i][k] == infinity || original[k][j] == infinity) ? infinity : original[i][k]+original[k][j];
								tmp[i][j] = min(original[i][j], sum);	
						}
				}
				for ( i = 0 ; i < n ; i++){
						for (j = 0; j < n; j++){
								original[i][j] = tmp[i][j];
						}
				}
				printMatrix(original, n);
				k++;
		}
		return tmp;

}
