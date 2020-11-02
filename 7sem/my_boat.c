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

void Passengers(int sem_id, int index)
{
//  printf("I'm passenger number %d, and I'm waiting for the trip on trap", index);

//  printf("I'm passenger number %d and I'm going to the boat!\n");

//  printf("I'm passenger number %d and I'm going ")

  printf("Hello, I'm passenger number %d", index);

  while(true)
  {
    //! Waiting for NULL
    Z_oper(sem_id, STAIR_DOWN);

    //! Passenger[index] go to elevator
    //! Took sem
    P_oper(sem_id, NUM_OF_STAIR, -1);
    printf("Passenger number %d have gone on the stair!\n", index);

    //! Passenger[index] go to boat
    //! Gave away sem
    V_oper(sem_id, NUM_OF_STAIR, 1);
    printf("Passenger number %d have gone to the boat", index);

    //! Took sem for passenger on boat
    P_oper(sem_id, NUM_ON_BOAT, -1);


    //! Passenger[index] waiting for the begging of his trip
    Z_oper(sem_id, START_OF_TRIP);

    //! Passenger[index] waiting for the ending of his trip
    Z_oper(sem_id, END_OF_TRIP);

    //! Passenger[index] waiting for the down of stair
    Z_oper(sem_id, STAIR_DOWN);

    //! Gave away sem for passenger on boat
    V_oper(sem_id, NUM_ON_BOAT, 1);

    //! Passenger[index] go to the ladder
    P_oper(sem_id, NUM_OF_STAIR, -1);
    printf("Passenger number %d have gone on the stair\n", index);

    //! Passenger[index] go to island
    V_oper(sem_id, NUM_OF_STAIR, 1);
    printf("Passenger number %d have gone on the island\n", index);


    P_oper(sem_id, GO_TO_COAST, -1);
  }

  printf("Passenger numver %d glad for trip on the boat!\n", index);

}

int main(int argc, char** argv)
{
  //! Getting arguments of terminal
  int num_of_pass = strtol(argv[1], NULL, 10)
      num_of_trips = strtol(argv[2], NULL, 10),
      cap_of_boat = strtol(argv[3], NULL, 10)
      cap_of_trap = strto(argv[4], NULL, 10);

  pid_t captains_pid = fork();

  int sem_id = semget(IPC_PRIVATE, num_of_semaphors, 0700);

  if (captains_pid == 0)
  {
    //! Add processing cap_of_boat and num_of_pass
    Captain(num_of_trips, sem_id);

    return 0;
  }
  else
  {
    perror("Captains pid not zero!\n");
    return ERROR;
  }

  for (int i = 0; i < num_of_pass; ++i)
  {
    pid_t passngrs_pid = fork();

    if (passngrs_pid)
    {
      Passengers(sem_id, i + 1);

      return 0;
    }
    else
    {
      perror("Passengers pid not zero!\n");
      return ERROR;
    }
  }

  return 0;
}
