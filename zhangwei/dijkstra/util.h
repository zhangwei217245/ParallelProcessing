#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define INFTY INT_MAX;

void printMatrix(int **edge, int n);
int **generateMatrix(int n);
int clk_gettime(int clk_id, struct timespec* t);
double lg(double n);
double flooor(double n);
int ceiling(double n);
char * getTimestamp();
char * getTimeString();
