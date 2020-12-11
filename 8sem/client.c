#include "semaphors.h"

char* shrd_mem_buf_cl = NULL;
int shrd_mem_id = 0;
int sem_id = 0;

//! Function for writing to shared memory
//!
int My_write(FILE* in_file)
{
  while (true)
  {
    //! Client take control under the channer!
    P_oper(sem_id, CHANNEL, 1);

    //! Then client write to the channel
    if ( (fgets(shrd_mem_buf_cl, BUFFER_SIZE, in_file)) == NULL)
    {
      //! Lock the channel for writing
      V_oper(sem_id, CHANNEL, 1);
      break;
    }

    //! Client add to the semaphor MEMORY and then waiting for NULL
    V_oper(sem_id, MEMORY, 1);
    Z_oper(sem_id, MEMORY);
  }

  return 1;
}


//! Use for shared memory with help functions:
//! ftok(), shmget(), shmat(), semget(), open(), fdopen()

int main(int argc, char** argv)
{
  int fd = 0;
  FILE* in_file = NULL;

  //! Getting id for shared memory segment <--> key_t key
  key_t key = ftok(NAME_OF_SERVER, SERVER_ID);

  if ((shrd_mem_id = shmget(key, BUFFER_SIZE, 0777)) < 0)
    return Err_proc("client: shmget return neg value!\n");

  if ((shrd_mem_buf_cl = (char*)shmat(shrd_mem_id, NULL, 0)) < 0)
    return Err_proc("client: shmat return neg value!\n");

  if ((sem_id = semget(key, num_of_semaphors, 0777)) < 0)
    return Err_proc("client: semget retur neg value!\n");

  //! Processing num of arguments

  if (argc == 1 || argv[1][0] == '-')
  {
    if (!My_write(stdin))
    {
      perror("client : My_write return error!\n");
      return errno;
    }


    return 0;
  }

  for (int i = 1; i < argc; ++i)
  {
    //! Creating file descriptor for working with file "something.dat"
    if ((fd = open(argv[i], O_RDONLY)) < 0)
    {
      perror("client : open return negative val to fd\n");
      return Err_proc(argv[i]);
    }

    if ((in_file = fdopen(fd, "rb")) < 0)
    {
      perror("client : fd return neg value to in_file!\n");
      return Err_proc(argv[i]);
    }

    if (!My_write(in_file))
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

  return 0;
}
