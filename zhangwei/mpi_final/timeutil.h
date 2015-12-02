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
char * getTimestamp();
char * getTimeString();
