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

int clk_gettime(int clk_id, struct timespec* t);
unsigned long getNanotime();
unsigned long getTimestamp();
char * getTimestampString();
char * getTimeString();
