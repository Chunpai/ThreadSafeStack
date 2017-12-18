# include "timing.h"

struct timeval tm1, tm2;
long long t;

void timing_start() {
    gettimeofday(&tm1, NULL);
}

void timing_stop() {
    gettimeofday(&tm2, NULL);

    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
}

void print_timing() {
    printf("%lld microseconds occured\n",t);
}

long long get_timing () {
    return t;
}
