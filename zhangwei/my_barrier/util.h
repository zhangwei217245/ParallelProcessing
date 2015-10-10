//Wei Zhang, Ali Nosrati

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int clk_gettime(int clk_id, struct timespec* t);
double lg(double n);
double flooor(double n);
int ceiling(double n);
char * getTimestamp();
char * getTimeString();
