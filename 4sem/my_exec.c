#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

const int ERROR = -1;

void Print_time(struct timespec start_clock, struct timespec final_clock)
{
  double secs = final_clock.tv_sec - start_clock.tv_sec;
  double nsecs = final_clock.tv_nsec - start_clock.tv_nsec;

  double time = secs*1000 + nsecs/1000000;

  printf("Process time in msecs : %lf\n", time);
}


int My_exec(char** argv)
{
  struct timespec start_clock = {0, 0}, final_clock = {0, 0};
  int status = 0;
  pid_t pid = fork();

  if (clock_gettime(CLOCK_REALTIME, &start_clock) != 0)
  {
    perror("Eror in clock_gettime, struct start_clock!\n");
    return ERROR;
  }

  //! if i am child then execut
  if (pid == 0)
  {
    execvp(argv[1], argv + 1);

    perror("Error with child!\n");
    return ERROR;
  }

  if (wait(&status) < 0)
  {
    perror("Wait return < 0");
    return ERROR;
  }

  //! add perror
  if (clock_gettime(CLOCK_REALTIME, &final_clock) != 0)
  {
    perror("Eror in clock_gettime, struct final clock!\n");
    return ERROR;
  }

  Print_time(start_clock, final_clock);
}



int main(int argc, char** argv)
{
  if (argc == 1)
  {
    printf("Lack of argumnets!\n");
    return 0;
  }

  else if (argc > 1)
  {
    My_exec(argv);
    return 0;
  }
}
