#include <time.h>
//clock_gettime is not implemented on OSX
int clk_gettime(int clk_id, struct timespec* t) {
    return clock_gettime(clk_id, t);
}
