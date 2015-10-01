#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

char * getTimestamp(){
	static char buff[20];
	struct timespec tps;
	clock_gettime(CLOCK_REALTIME, &tps);
	unsigned long now = tps.tv_sec * 1000 + tps.tv_nsec / 1000000;
	sprintf(buff, "%lu", now);
	return buff;
}

int main(int argc, char *argv[])
{
    
}
