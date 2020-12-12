#include <stdio.h>

#include <sys/times.h>

#include <time.h>


void Print_time(struct timespec start_clock, struct timespec final_clock)
{
  double secs = final_clock.tv_sec - start_clock.tv_sec;
  double nsecs = final_clock.tv_nsec - start_clock.tv_nsec;

  double time = secs*1000 + nsecs/1000000;

  printf("Process time in msecs : %lf\n", time);
}


int main(int argc, char** argv)
{
  
}
