#include <stdio.h>
#include "util.h"

int main(int argc, char * argv[]){
		int n = 5;
		int **edge = generateMatrix(n);
		printMatrix(edge, n);
}
