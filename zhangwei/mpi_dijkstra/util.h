/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#define INFTY INT_MAX;

void printMatrix(int **edge, int n);
int **generateMatrix(int n);
int clk_gettime(int clk_id, struct timespec* t);
double lg(double n);
double flooor(double n);
int ceiling(double n);
char * getTimestamp();
char * getTimeString();
int min(int i1, int i2);
int * appendIntArray(int *arr1, int size1, int *arr2, int size2);
