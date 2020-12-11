#include "semaphor.h"

//! Function for writing to shared memory
int My_write(int fd)
{
  //! TO DO: realisation
}



//! Use for shared memory with help functions:
//! ftok(), shmget(), shmat(), semget(), open(), fdopen()

int main(int argc, char** argv)
{
  int fd = 0, FILE* in_file = NULL;

  //! Getting id for shared memory segment <--> key_t key
  key_t key = ftok(NAME_OF_SERVER, SERVER_ID);

  if ((shrd_mem_id = shmget(key, BUFFER_SIZE, 0777)) < 0)
    return Err_proc("client: shmget return neg value!\n");

  if ((semaphs_id = semget(key, num_of_semaphors, 0777)) < 0)
    return Err_proc("client: semget retur neg value!\n");

  //! Processing num of arguments

  if ((argv[1][0] == '-') || (argc == 1))
  {
    My_write(stdin);

    return 0;
  }

  for (int i = 1; i < argc; ++i)
  {
    //! Creating file descriptor for working with file "something.dat"
    if ((fd = open(argv[i], O_RDONLY)) < 0)
    {
      perror("client : open return negative val to fd\n")
      return Err_proc(argv[i]);
    }

    if ((in_file = fdopen(fd, "rb")) < 0)
    {
      perror("client : fd return neg value to in_file!\n");
      return Err_proc(argv[i]);
    }

    if (My_write(in_file) == ERROR)
    {
      perror("client : My_write return error!\n");
      return Err_proc(argv[i]);
    }

    if (fclose(in_file) < 0)
    {
      perror("client : fclose return neg value!\n");
      return Err_proc(argv[i]);
    }

  }





}
