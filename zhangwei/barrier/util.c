#include "util.h"
#include <math.h>

int clk_gettime(int clk_id, struct timespec* t) {
		return clock_gettime(clk_id, t);
}
double lg(double n){
		return log(n)/log(2);
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
