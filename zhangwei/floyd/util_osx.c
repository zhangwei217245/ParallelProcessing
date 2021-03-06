#include "util.h"
#include <math.h>
#include <sys/time.h>
//clock_gettime is not implemented on OSX
#define CLOCK_REALTIME 0

void printMatrix(int **edge, int n){
		int i, j, infinity = INFTY;
		for(i = 0; i < n; i++){
				for (j = 0; j < n; j++){
						if ( infinity == edge[i][j] ){
								printf("INFTY\t");
						} else {
								printf("%d\t", edge[i][j]);
						}
				}
				printf("\n");
		}
		printf("\n");
}
int **generateMatrix(int n){
		int i, j;
		//Initializing Matrix
		int **edge;
		edge=(int **) calloc(sizeof(int *), n);
		for (i = 0; i < n; i++){
				edge[i] = (int *) calloc (sizeof(int),n);
		}
		//Fill Matrix with Random number with a few of them being INFTY, keep the element on diagonal as zero,
		srand((unsigned int) time(NULL));
		for (i = 0; i < n; i++){
				int infPos = rand() % n;//randomly insert a INFTY in each row.
				if ( infPos == 0 || infPos == (n-1)){
						infPos = -1;//The first and last element shouldn't be INFTY
				}
				for (j = 0; j < n; j++){
						if (i == j){
								edge[i][j] = 0;
						} else if (j == infPos){
								edge[i][j] = INFTY;
						} else {
								edge[i][j] = rand() % 20;
						}
				}		
		}
		return edge;
}
int clk_gettime(int clk_id, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}

double lg(double n){
		return log(n)/log(2);
}
double flooor(double n){
				return floor(n);
}
int ceiling(double n){
		return (int)ceil(n);
}

char * getTimestamp(){
		static char buff[20];
		struct timespec tps;
		clk_gettime(CLOCK_REALTIME, &tps);
		unsigned long now = tps.tv_sec * 1000 + tps.tv_nsec / 1000000;
		sprintf(buff, "%lu", now);
		return buff;
}
char * getTimeString(){
		static char buff[20];
		struct timespec tps;
		clk_gettime(CLOCK_REALTIME, &tps);
		unsigned long now_milli = tps.tv_nsec / 1000000;
		time_t now = tps.tv_sec;
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
		sprintf(buff, "%s:%03lu", buff, now_milli);
		return buff;
}
int min(int i1, int i2){
		if (i2 < i1){
				return i2;
		} else {
				return i1;
		}
}
