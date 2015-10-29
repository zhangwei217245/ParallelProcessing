#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#define INFTY INT_MAX;

void printMatrix(int **edge, int n);
int **generateMatrix(int n);
int safesum(int s1, int s2);
double lg(double n);
double flooor(double n);
int ceiling(double n);
int min(int i1, int i2);
