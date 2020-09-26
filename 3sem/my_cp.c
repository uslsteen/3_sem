#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

const int BUF_SIZE = 4096;

//! Enum for beatiful names of errors
enum ERRORS
{
  NEGATIVE_VAR = 0,
  FUNC_PRCSS_ERROR = 0
};


//! Upgraded func write(...)
//! @param[in] fd - file descriptor
//! @param[in] buffer for writing from buffer to fd
//! @param[in] res_of_read - result of one iteration my_read func
//! @return true if func executed rigth
bool My_write(int fd, char* buffer, int res_of_read)
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
      char buffer[BUF_SIZE];

      if ((res_of_read = read(fd_srce, buffer, BUF_SIZE)) < 0)
          return NEGATIVE_VAR;

      if (!My_write(fd_dest, buffer, res_of_read))
      {
          printf("Error with writing from buffer to fd\n");
          return FUNC_PRCSS_ERROR;
      }
    } while (res_of_read != 0);

    return true;
}


bool Simple_copy(int argc, char** argv)
{
  int fd_srce = open(argv[1], O_RDONLY);

  if (fd_srce < 0)
  {
    printf("cat: %s: Нет такого файла или каталога\n", argv[1]);
    return NEGATIVE_VAR;
  }

  int fd_dest = open(argv[2], O_WRONLY);

  if (fd_dest < 0)
  {
    fd_dest = open(argv[2], O_CREAT);
  }

  My_read(fd_srce, fd_dest);

  return true;
}


int main(int argc, char** argv)
{
   if (!Simple_copy(argc, argv))
    return 1;

  return 0;
}
