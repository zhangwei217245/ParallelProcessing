#include <stdio.h>
#include "util.h"
#include "floyd.h"


int main(int argc, char * argv[]){
		int n = 5, i;
		int **edge = generateMatrix(n);
		printMatrix(edge, n);
		int **result = floyd(n, edge);


}
