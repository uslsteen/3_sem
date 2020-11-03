/*
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <sys/sem.h>
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

/*
const int num_of_people = 10;
const int cap_of_boat = 2;
 const int num_of_travels = 5;
*/

const int num_of_semaphors = 6;

const int ERROR = -1;

enum Semaphors
{
  STAIR_DOWN = 0,
  START_OF_TRIP,
  NUM_ON_BOAT,
  NUM_ON_STAIR,
  END_OF_TRIP,
  GO_TO_COAST,
  LAST_TRIP
};


//! Semaphors operation: p(c, 15) <=> minus 15 to semaphor c,
//!                      v(c, 15) <=> plus 15 to semaphor c,
//!                      z waiting for NULL


void P_oper(int sem_id, enum Semaphors sem_num, int num)
{
  struct sembuf sem_struct = {sem_num, -num, 0};

  if (semop(sem_id, &sem_struct, 1) < 0)
  {
    perror("sem op return not zero in P_oper!\n");
    exit(0);
  }
}

void V_oper(int sem_id, enum Semaphors sem_num, int num)
{
  struct sembuf sem_struct = {sem_num, num, 0};

  if (semop(sem_id, &sem_struct, 1) < 0)
  {
    perror("sem op return not zero in V_oper!\n");
    exit(0);
  }
}

void Z_oper(int sem_id, enum Semaphors sem_num)
{
  struct sembuf sem_struct = {sem_num, 0, 0};

  if (semop(sem_id, &sem_struct, 1) < 0)
  {
    perror("sem op return not zero in Z_oper!\n");
    exit(0);
  }
}
