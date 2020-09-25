#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#define BUF_SIZE 4096

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

    while (res_of_write < res_of_read)
    {
        res_of_write = write(fd, buffer, res_of_read - written_size);

        if (res_of_write < 0)
        {
          printf("Negative res of write!\n");
          return NEGATIVE_VAR;
        }

        written_size += res_of_write;
        buffer += written_size;
    }

    return true;
}


//! Upgraded func read(...)
//! @param[in] fd - file descriptor
//! @return true if func executed rigth
bool My_read(int fd)
{
    int res_of_read = 0;

    do
    {
      char buffer[BUF_SIZE];

      if ((res_of_read = read(fd, buffer, BUF_SIZE)) < 0)
          return NEGATIVE_VAR;

      if (!My_write(STDOUT_FILENO, buffer, res_of_read))
      {
          printf("Error with writing from buffer to fd");
          return FUNC_PRCSS_ERROR;
      }
    } while (res_of_read != 0);

    return true;
}



int main( int argc, char * argv[] )
{
  if (argc == 1 || argv[1][0] == '-')
  {
    if (!My_read(STDIN_FILENO))
      return FUNC_PRCSS_ERROR;

    return 0;
  }

  for (int i = 1; i < argc; ++i)
  {
    int fd = open(argv[i], O_RDONLY);

    if (fd < 0)
    {
      printf("cat: %s: Нет такого файла или каталога\n", argv[i]);
      return NEGATIVE_VAR;
    }

    if (!My_read(fd))
      return FUNC_PRCSS_ERROR;

    if (close(fd) < 0)
    {
        printf("Error close func!\n");
        return FUNC_PRCSS_ERROR;
    }
  }

  return 0;
}
