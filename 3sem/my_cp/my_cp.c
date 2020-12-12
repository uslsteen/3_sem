#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096

#define MAX_ACCESS 0777

//! Keys bits constants
#define F_KEY 0x1
#define I_KEY 0x2
#define V_KEY 0x4

//! Typeder for working with getopt structure
typedef struct option OPT;

enum
{
  INV_KEY = -2,
  END_OF_KEYS = -1
};

const OPT OPTIONS[] =
{
    {"force", 0, NULL, 'f'},
    {"interactive", 0, NULL, 'i'},
    {"verbose", 0, NULL, 'v'},
    {0, 0, 0, 0},
};

const char OPT_STR[] = "fiv";



int Err_proc(const char* err_string)
{
  perror(err_string);
  return errno;
}

bool My_write(int fd, const char* buffer, int res_of_read);
bool My_read(int fd_srce, int fd_dest);

int Get_options(int argc, char** argv);

void Print_verb(const char* src, const char* dst);

void Write_prompt(const char* dst);
int Get_prompt(const char* dst);

int Dst_process(int* fd_dst, const char* dst, int Is_F, int Is_I);

int Copy_file(const char* src, const char* dst, int flags);

int Check_dir(const char* name_of_file);

int Buffer_processing(int flags, char** names, size_t size);

int main(int argc, char** argv)
{
  int flags = Get_options(argc, argv);

  if (flags == INV_KEY)
    return 1;

  if (argc - optind < 2)
  {
    printf("Lack of arguments: %d\n", argc - optind);
    return 1;
  }

  int dir_checker = Check_dir(argv[argc - 1]);

  if (dir_checker > 0)
    return 1;

  if (dir_checker < 0 && Buffer_processing(flags, argv + optind, argc - optind))
    return 1;

  if (!dir_checker && Copy_file(argv[optind], argv[optind + 1], flags))
    return 1;

  return 0;
}




//! Upgraded func write(...)
//! @param[in] fd - file descriptor
//! @param[in] buffer for writing from buffer to fd
//! @param[in] res_of_read - result of one iteration my_read func
//! @return true if func executed rigth
bool My_write(int fd, const char* buffer, int res_of_read)
{
    int res_of_write = 0, written_size = 0;

    do
    {
        res_of_write = write(fd, buffer, res_of_read - written_size);

        if (res_of_write < 0)
        {
          printf("Negative res of write!\n");
          return false;
        }

        written_size += res_of_write;
        buffer += written_size;

    } while (res_of_write < res_of_read);

    return true;
}

//! Upgraded func read(...)
//! @param[in] fd - file descriptor
//! @return true if func executed rigth
bool My_read(int fd_srce, int fd_dest)
{
    int res_of_read = 0;

    do
    {
      char buffer[BUFFER_SIZE];

      if ((res_of_read = read(fd_srce, buffer, BUFFER_SIZE)) < 0)
          return false;

      if (!My_write(fd_dest, buffer, res_of_read))
      {
          printf("Error with writing from buffer to fd\n");
          return false;
      }
    } while (res_of_read != 0);

    return true;
}


//! Function for processing key from stdin
//! There is processing keys:
//! -f:
//! -i:
//! -v:
int Get_options(int argc, char** argv)
{
  int getopt_res = 0;
  int flag = 0;

  while (1)
  {
    getopt_res = getopt_long(argc, argv, OPT_STR, OPTIONS, NULL);

    switch (getopt_res)
    {
    case '?':
      return INV_KEY;
    case 'f':
      flag |= F_KEY;
      break;
    case 'i':
      flag |= I_KEY;
      break;
    case 'v':
      flag |= V_KEY;
      break;
    case -1:
      return flag;
    default:
      printf("Unrecognized return value: %d\n", getopt_res);
      return INV_KEY;
    }
  }
}


//!
void Print_verb(const char *src, const char *dst)
{
  printf("\'%s\' -> \'%s\'\n", src, dst);
}

void Write_prompt(const char* dst)
{
  const char fst[] = "cp: rewrite '";
  const char sec[] = "'? ";

  My_write(STDOUT_FILENO, fst, sizeof(fst));
  My_write(STDOUT_FILENO, dst, strlen(dst));
  My_write(STDOUT_FILENO, sec, sizeof(sec));
}

int Get_prompt(const char* dst)
{
  Write_prompt(dst);

  char buffer[BUFFER_SIZE];

  if (read(STDIN_FILENO, buffer, BUFFER_SIZE) < 0)
    return Err_proc("cp: Error with stdin: ");

  if (toupper(buffer[0]) == 'Y')
    return 0;

  return -1;
}

//! Function for processing dst file (also dest file descriptor)
//! Also there is processing keys from stdin: -f, -i
int Dst_process(int *fd_dst, const char *dst, int Is_F, int Is_I)
{
  if (*fd_dst < 0)
  {
    if (errno == EEXIST && (Is_I || Is_F))
    {
      int Is_cp = 1;

      if (Is_I)
        Is_cp = (!Get_prompt(dst));

      if (!Is_cp)
        return 1;

      *fd_dst = open(dst, O_WRONLY, MAX_ACCESS);

      if (*fd_dst < 0)
        return Err_proc(dst);

      return 0;
    }

    return Err_proc(dst);
  }
  return 0;
}

//! Function for copy from src file to dst file
int Copy_file(const char *src, const char* dst, int flags)
{
  int Is_F = (flags & F_KEY) && 1,
      Is_I = (flags & I_KEY) && 1,
      Is_V = (flags & V_KEY) && 1;

  int fd_src = open(src, O_RDONLY, MAX_ACCESS);

  if (fd_src < 0)
    return Err_proc(src);

  int fd_dst = open(dst, O_WRONLY | O_CREAT | O_EXCL, MAX_ACCESS);

  if (Dst_process(&fd_dst, dst, Is_F, Is_I) || My_read(fd_src, fd_dst))
    return -1;

  if (Is_V)
    Print_verb(src, dst);

  if (close(fd_dst) < 0)
    return Err_proc(dst);

  if (close(fd_src) < 0)
    return Err_proc(src);

  return 0;
}

//! Function for checking: directory or not
int Check_dir(const char* name_of_file)
{
  int fd = open(name_of_file, O_DIRECTORY);

  if (fd < 0)
  {
    //! In case file
    if (errno == ENOTDIR)
      return 0;

    return Err_proc(name_of_file);
  }

  if (close(fd) < 0)
    return Err_proc(name_of_file);

  //! In case directory
  return -1;
}

//! Function for processing buffer[BUFFER_SIZE]
int Buffer_processing(int flags, char *names[], size_t size)
{
  char buffer[BUFFER_SIZE];
  strcpy(buffer, names[size - 1]);

  size_t path_size = strlen(names[size - 1]);

  buffer[path_size++] = '/';
  buffer[path_size] = '\0';

  for (size_t i = 0; i < size - 1; ++i)
  {
    strcat(buffer, names[i]);
    Copy_file(names[i], buffer, flags);

    buffer[path_size] = '\0';
  }

  return 0;
}
