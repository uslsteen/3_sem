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
const int STD_IN = 0;
const int BUF_SIZE = 4096;

enum Options
{
  WITH_ARGS = 0,
  WITHOUT_ARGS = 1
};

enum Errors
{
  NEGATIVE_VALUE = 0,
};

struct Counters
{
  int num_of_word;
  int num_of_str;
  int num_of_bytes;
};

struct options OPTS
{
  "w_cntr", 0, NULL, 'w'
}

const char LONGOPTS = "";

int Get_options(int argc, char** argv)
{
  int getopt_res = 0;
  int opt_checker = 0;

  while(true)
  {
    getopt_res = getopt_long(argc, argv, optstring, LONGOPTS, NULL);

    switch(getopt_res)
    {
      case 'w': opt_checker = WITH_ARGS;
        break;

      case -1: opt_checker = WITHOUT_ARGS;
        break;

      default: perror("Undefined options!\n");
        break;
    }
  }
}

int Word_counter(char* buffer, int buf_size, int* pos_in_word)
{
  int i = 0, num_of_words = 0;

  for (i = 0; i < buf_size; ++i)
  {
    if (!isspace(buffer[i]) && (pos_in_word == 0))
    {
      num_of_words++;
      *pos_in_word = 1;
    }
    else if (isspace(buffer[i]))
      *pos_in_word = 0;
  }

  return num_of_words;
}

int Str_counter(char* buffer, int buf_size)
{
  int i = 0, num_of_str = 0;

   for (i = 0; i < buf_size; ++i)
   {
     if(buffer[i] == '\n')
      num_of_str++;

    buffer++;
   }
}

int Counter(int fd, struct Counters* cntrs)
{
  int res_read_bytes = 0;
  int pos_in_word = 0;

  do
  {
    char buffer[BUF_SIZE];

    if ((res_read_bytes = read(fd, buffer, BUF_SIZE)) < 0)
    {
      perror("Read return negative value!\n");
      return NEGATIVE_VALUE;
    }

    cntrs->num_of_bytes += res_read_bytes;
    cntrs->num_of_word += Word_counter(buffer, res_read_bytes, &pos_in_word);//;
    cntrs->num_of_str += Str_counter(buffer, res_read_bytes);//;

  } while(res_read_bytes > 0);

}

void Print_time(struct timespec start_clock, struct timespec final_clock)
{
  double secs = final_clock.tv_sec - start_clock.tv_sec;
  double nsecs = final_clock.tv_nsec - start_clock.tv_nsec;

  double time = secs*1000 + nsecs/1000000;

  printf("Process time in msecs : %lf\n", time);
}


int My_exec(char** argv, int opt)
{
  struct timespec start_clock = {0, 0}, final_clock = {0, 0};
  int status = 0;
  int fd[2];
  struct Counters cntrs = {0, 0, 0};

  if (pipe(fd) == -1)
  {
    perror("Pipe return error\n");
    return ERROR;
  }

  pid_t pid = fork();

  if (clock_gettime(CLOCK_REALTIME, &start_clock) != 0)
  {
    perror("Eror in clock_gettime, struct start_clock!\n");
    return ERROR;
  }

  if (pid > 0)
  {
    if (close(fd[1] == -1))
    {
      perror("Close return -1\n");
      return ERROR;
    }

    Counter(fd[0], &cntrs);

    printf("Result of word counter:\n");
    printf("%d ", cntrs.num_of_str);
    printf("%d ", cntrs.num_of_word);

    close(fd[0]);
  }


  //! if i am child then execut
  if (pid == 0)
  {

    if (opt == WITH_ARGS)
    {
      if (close(fd[0]) == -1)
      {
        perror("Close return -1\n");
        return ERROR;
      }

      fd[1] = dup2(fd[1], STD_OUT);


      close(fd[1]);
      //printf("debug printf fd[0] = %d, fd[1] = %d\n", fd[0], fd[1]);
    }

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
    int opt = Get_options(argc, argv);

    My_exec(argv, opt);
    return 0;
  }
}
