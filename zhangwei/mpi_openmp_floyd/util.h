/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <mpi.h>
#include "omp.h"

#define INFTY 100;
//INT_MAX;

void printMatrix(int **edge, int n);
int **generateMatrix(int n);
int safesum(int s1, int s2);
double lg(double n);
double flooor(double n);
int ceiling(double n);
void getSizeAndRank(int *size, int *rank, MPI_Comm comm);
int min(int i1, int i2);
void printArray(int *arr, int n);
