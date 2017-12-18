# include <sys/time.h>
# include <stdio.h>

# ifndef TIMING_H
# define TIMING_H 1

void timing_start (void);
void timing_stop  (void);
void print_timing (void);
long long get_timing (void);

# endif
