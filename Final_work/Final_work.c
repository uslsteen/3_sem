#include <stdio.h>

#include <sys/times.h>

#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <assert.h>


int Err_proc(char* err_string)
{
  perror(err_string);
  return errno;
}

void Print_real_time(struct timespec start_clock, struct timespec final_clock)
{
  double secs = final_clock.tv_sec - start_clock.tv_sec;
  double nsecs = final_clock.tv_nsec - start_clock.tv_nsec;

  double time = secs*1000 + nsecs/1000000;

  printf("Process time in msecs : %lf\n", time);
}





void My_time(int argc, char** argv)
{
  struct timespec start_clock = {0, 0}, final_clock = {0, 0};
  int status = 0;
  struct tms another_time;
  long long int num_of_ticks = sysconf(_SC_CLK_TCK);

  if (clock_gettime(CLOCK_REALTIME, &start_clock) != 0)
    Err_proc("Error in clock_gettime, start clock!\n");

  pid_t pid = fork();

  //! if i am child then execut
  if (pid == 0)
  {
    execvp(argv[1], argv + 1);

    perror("Error with child!\n");
    return errno;
  }

  if (wait(&status) < 0)
    Err_proc("Wait return < 0!\n");

  if (clock_gettime(CLOCK_REALTIME, &final_clock) != 0)
    Err_proc("Error in clock_gettime, start clock!\n");

  int check = times(&another_time);

  Print_real_time(start_clock, final_clock);


}


int main(int argc, char** argv)
{
  if (argc < 2)
  {
    perror("Lack arguments!\n");
    exit(0);
  }
  else
  {
    My_time(argc, argv);
    return 0;
  }
}
