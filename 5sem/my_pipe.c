#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <getopt.h>
#include <ctype.h>

const int ERROR = -1;
const int WITH_OPT = 1;
const int BUF_SIZE = 4096;

//! Structure for counter function
struct Counter_t
{
  int num_of_bytes;
  int num_of_word;
  int num_of_str;
};


struct option longopts[] =
{
  {"w", 0, NULL, 'w'},
  {0, 0, 0, 0},
};

const char optstring[] = "";

//! Function for proseccing options
int Getopt_process(int argc, char** argv);

//! Function for print time of execute
void Print_time(struct timespec start_clock, struct timespec final_clock);

//! Executuion function
int My_exec(char** argv, bool opt);


int Word_counter(char* buffer, int buf_size, bool* pos_in_word);
int Str_counter(char* buffer, int buf_size);
int Counter_func(int fd, struct Counter_t* cntrs);


int main(int argc, char** argv)
{
  if (argc == 1)
  {
    printf("Lack of argumnets!\n");
    return 0;
  }

  else if (argc > 1)
  {
    int opt = Getopt_process(argc, argv);

    My_exec(argv, opt);
    return 0;
  }
}


int Getopt_process(int argc, char** argv)
{
  int getopt_res = 0;
  int opt_checker = 0;

  while (true)
  {
    getopt_res = getopt_long(argc, argv, optstring, longopts, NULL);

    switch (getopt_res)
    {
      case -1:
          return opt_checker;

      case 'w':
        opt_checker = WITH_OPT;
          return opt_checker;

      default:
        printf("Undefined options %d!\n", getopt_res);
          return opt_checker;
    }
  }
}


void Print_time(struct timespec start_clock, struct timespec final_clock)
{
  double secs = final_clock.tv_sec - start_clock.tv_sec;
  double nsecs = final_clock.tv_nsec - start_clock.tv_nsec;

  double time = secs*1000 + nsecs/1000000;

  printf("Process time in msecs : %lf\n", time);
}


int My_exec(char** argv, bool opt)
{
  struct timespec start_clock = {0, 0}, final_clock = {0, 0};
  int status = 0;
  int fd[2];

  pipe(fd);

  pid_t pid = fork();

  if (clock_gettime(CLOCK_REALTIME, &start_clock) != 0)
  {
    perror("Eror in clock_gettime, struct start_clock!\n");
    return ERROR;
  }

  //! if i am child then execut
  if (pid == 0)
  {
    printf("fd[0] = %d, fd[1] = %d\n", fd[0], fd[1]);

    if (opt)
    {
      if (close(fd[0] == -1))
      {
        perror("Error in close fd[0]\n");
        return ERROR;
      }

      fd[1] = dup2(fd[1], STDOUT_FILENO);
      printf("fd[0] = %d, fd[1] = %d\n", fd[0], fd[1]);
    }

    execvp(argv[optind], argv + optind);

    exit(0);
  }

  else if (pid > 0)
  {
    struct Counter_t cntr = {0, 0, 0};

    if (opt)
    {
      if (close(fd[1]) == -1)
      {
        perror("Error in close fd[1]\n");
        return ERROR;
      }

      Counter_func(fd[0], &cntr);

      if (close(fd[0]) == -1)
      {
        perror("Error in close fd[0]\n");
        return ERROR;
      }

      printf("Relust of counter func:\n");
      printf("bytes = %d\nwords = %d\nstrings = %d\n", cntr.num_of_bytes, cntr.num_of_word, cntr.num_of_str);
    }
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

  return 0;
}

int Word_counter(char* buffer, int buf_size, bool* pos_in_word)
{
  int i = 0, num_of_words = 0;

  for (i = 0; i < buf_size; ++i)
  {
    if (!isspace(buffer[i]) && !(*pos_in_word))
    {
      num_of_words++;
      *pos_in_word = true;
    }
    else if (isspace(buffer[i]))
      *pos_in_word = false;
  }

  return num_of_words;
}

int Str_counter(char* buffer, int buf_size)
{
  int i = 0, num_of_str = 0;

   for (i = 0; i < buf_size; ++i)
   {
     if (buffer[i] == '\n')
      num_of_str++;
   }

   return num_of_str;
}

int Counter_func(int fd, struct Counter_t* cntrs)
{
  int res_read_bytes = 0;
  bool pos_in_word = false;

  do
  {
    char buffer[BUF_SIZE];

    if ((res_read_bytes = read(fd, buffer, BUF_SIZE)) < 0)
    {
      perror("Read return negative value!\n");
      return -1;
    }

    printf("res_of_read = %d\n", res_read_bytes);

    cntrs->num_of_bytes += res_read_bytes;
    cntrs->num_of_word += Word_counter(buffer, res_read_bytes, &pos_in_word);//;
    cntrs->num_of_str += Str_counter(buffer, res_read_bytes);//;

  } while(res_read_bytes > 0);

  return 0;
}
