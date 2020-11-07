#include "semaphors.h"

void Passengers(int sem_id, int index)
{
  bool flag = false;

  while (true)
  {
    //! Passenger[index] is waiting for stair
    Z_oper(sem_id, STAIR_DOWN);

    if (semctl(sem_id, LAST_TRIP, GETVAL))
      break;

    printf("Passenger number %d booked a ticket on the boat!\n", index);
    P_oper(sem_id, NUM_ON_BOAT, 1);

    if (semctl(sem_id, LAST_TRIP, GETVAL))
      break;

    P_oper(sem_id, NUM_ON_STAIR, 1);
    printf("Passenger number %d stepped in the stair!\n", index);

    V_oper(sem_id, NUM_ON_STAIR, 1);
    printf("Passenger number %d stepped into the boat!\n", index);

    flag = true;

    //! Passenger[index] is waiting for the begging of his trip
    Z_oper(sem_id, START_OF_TRIP);

    //! Passenger[index] is waiting for the ending of his trip
    Z_oper(sem_id, END_OF_TRIP);

    //! Passenger[index] is waiting for the down of stair
    Z_oper(sem_id, STAIR_DOWN);

    //! Passenger[index] go away from boat to the stair
    P_oper(sem_id, NUM_ON_STAIR, 1);
    printf("Passenger number %d have gone on the stair\n", index);

    //! Passenger[index] go away from stair to the island
    V_oper(sem_id, NUM_ON_STAIR, 1);
    printf("Passenger number %d have gone on the island\n", index);

    //! Another place on the boat is free now
    V_oper(sem_id, NUM_ON_BOAT, 1);


    P_oper(sem_id, NUM_ON_COAST, 1);
  }

  if (flag)
    printf("Passenger number %d is glad for trip on the boat!\n", index);

  else
    printf("I passenger number %d bought ticket, but I didnt visit the boat!\n", index);
}


void Captain(int sem_id, int num_of_trips, int cap_of_stair, int cap_of_boat)
{
  //! Semaphors init
  P_oper(sem_id, STAIR_DOWN, 1);
  P_oper(sem_id, START_OF_TRIP, 1);

  for (int i = 0; i < num_of_trips; ++i)
  {
    if (i == num_of_trips - 1)
      V_oper(sem_id, LAST_TRIP, 1);

    //! Boat preparing
    printf("Hello, I'm your captain!\n");
    printf("Stair is going down!\n");

    printf("Welcome to the boat!\n");

    Z_oper(sem_id, NUM_ON_BOAT);

    V_oper(sem_id, STAIR_DOWN, 1);

    printf("Captain is waiting for the stair to be removed\n");
    printf("Stair is going up!\n");

    P_oper(sem_id, NUM_ON_STAIR, (short int)cap_of_stair);
    V_oper(sem_id, NUM_ON_STAIR, cap_of_stair);

    printf("Let's start our trip!\n");
    V_oper(sem_id, START_OF_TRIP, 1);

    printf("        *** Two hours later ***        \n");

    P_oper(sem_id, END_OF_TRIP, 1);
    printf("Ladies ang gentelments, our trip is over!\nGo away from my boat!\n");

    //! Captain down the stair
    P_oper(sem_id, STAIR_DOWN, 1);

    Z_oper(sem_id, NUM_ON_COAST);

    //! Captain is preparing boat for new quant of pepople
    printf("Wow, the boat is finally empty. I have finished another journey.\n");
    V_oper(sem_id, END_OF_TRIP, 1);

    V_oper(sem_id, NUM_ON_COAST, cap_of_boat);

    if (i == num_of_trips - 1)
      break;

    P_oper(sem_id, START_OF_TRIP, 1);
  }

  printf("Captain go away!\n");
}

void Sems_processing(int sem_id, int cap_of_boat, int num_of_pass, int cap_of_stair)
{
  V_oper(sem_id, STAIR_DOWN, 1);
  V_oper(sem_id, END_OF_TRIP, 1);
  V_oper(sem_id, START_OF_TRIP, 1);
  V_oper(sem_id, NUM_ON_STAIR, cap_of_stair);

  if (cap_of_boat < num_of_pass)
  {
    V_oper(sem_id, NUM_ON_BOAT, cap_of_boat);
    V_oper(sem_id, NUM_ON_COAST, cap_of_boat);
  }
  else
  {
    V_oper(sem_id, NUM_ON_BOAT, num_of_pass);
    V_oper(sem_id, NUM_ON_COAST, num_of_pass);
  }
}



int main(int argc, char** argv)
{
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
  pid_t captain_pid = 0, passngrs_pid = 0;

  int num_of_pass = 10,
    num_of_trips = 3,
    cap_of_boat = 4,
    cap_of_stair = 2;

  if (argc >= 5)
  {
  //! Getting arguments of terminal
    num_of_pass = strtol(argv[1], NULL, 10),
    num_of_trips = strtol(argv[2], NULL, 10),
    cap_of_boat = strtol(argv[3], NULL, 10),
    cap_of_stair = strtol(argv[4], NULL, 10);
  }

  int sem_id = semget(IPC_PRIVATE, num_of_semaphors, 0777);
  if (sem_id < 0)
  {
      perror("Semget return error!\n");
      return ERROR;
  }

  Sems_processing(sem_id, cap_of_boat, num_of_pass, cap_of_stair);


  if ((captain_pid = fork()) < 0)
  {
    perror("Fork retun neg value to captain pid!\n");
    exit(0);
  }

  if (captain_pid == 0)
  {
    if (cap_of_boat < num_of_pass)
      Captain(sem_id, num_of_trips, cap_of_stair, cap_of_boat);

    else
      Captain(sem_id, num_of_trips, cap_of_stair, num_of_pass);

    return 0;
  }


  for (int i = 0; i < num_of_pass; ++i)
  {
    if ((passngrs_pid = fork()) < 0)
    {
      perror("Fork retun neg value to passgrs pid!\n");
      exit(0);
    }

    if (passngrs_pid == 0)
    {
      Passengers(sem_id, i + 1);
      return 0;
    }
  }

  return 0;
}
