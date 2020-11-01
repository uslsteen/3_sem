#include "my_sem.h"

//! Heroes of this program

void Captain(int num_of_trips, int sem_id)
{
    //! Boat preparing
    printf("Hello, I'm your captain!\n");
    printf("Trap is going down!\n");

    for (int i = 0; i < num_of_trips; ++i)
    {
      printf("Welcome to the boat!\n");

      //! Passengers go to boat
      //! Captain waiting for passangers

      printf("Enough! Trap is going up!\n");
      printf("Let's start our trip!\n");

      printf("        *** Two hours later ***        \n");

      printf("Ladies ang gentelments, our trip is over!\nGo away from my boat!\n");

      //! Reloading of boat

    }


}

void Passengers(int index)
{
  printf("I'm passenger number %d, and I'm waiting for the trip on trap", index);

  printf("I'm passenger number %d and I'm going to the boat!\n");

  printf("I'm passenger number %d and I'm going ")

}

int main(int argc, char** argv)
{
  //! Getting arguments of terminal
  int num_of_passngrs = strtol(argv[1], NULL, 10)
      num_of_trips = strtol(argv[2], NULL, 10),
      cap_of_boat = strtol(argv[3], NULL, 10)
      cap_of_trap = strto(argv[4], NULL, 10);

  pid_t captains_pid = fork();
  int status;

  int sem_id = semget(IPC_PRIVATE, num_of_semaphors, 0700);

  if (captains_pid == 0)
  {
    Captain(num_of_trips, sem_id);

    //! When captain finish his job
    exit(0);
  }
  else
  {
    perror("Captains pid not zero!\n");
    return ERROR;
  }

  for (int i = 0; i < num_of_passngrs; ++i)
  {
    pid_t passngrs_pid = fork();

    if (passngrs_pid)
    {
      Passengers(i);
    }
    else
    {
      perror("Passengers pid not zero!\n");
      return ERROR;
    }
  }

  for (int i = 0; i < num_of_passngrs + 1; ++i)
  {
    if (wait(&status) < 0)
    {
      perror("Wait return not zero!\n");
      return ERROR;
    }
  }

  return 0;
}
