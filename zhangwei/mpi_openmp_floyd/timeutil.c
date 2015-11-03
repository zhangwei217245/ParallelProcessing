/**
 *  Author: Wei Zhang
 *  Author: Ali Nosrati
 *
 *
 */
#include "timeutil.h"

int clk_gettime(int clk_id, struct timespec* t) {
		return clock_gettime(clk_id, t);
}

unsigned long getTimestamp(){
		struct timespec tps;
		clk_gettime(CLOCK_REALTIME, &tps);
		unsigned long now = tps.tv_sec * 1000 + tps.tv_nsec / 1000000;
		return now;
}

char * getTimestampString(){
		static char buff[20];
		unsigned long now = getTimestamp();
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

