#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>


const int num_of_semaphors = 5;
const int ERROR = -1;


Shared int Upwait = 0, Upgo = 0, Downgo = 0, Cap = 0;

enum Semaphors
{
  LOCK_UP = 0, //! Sem for going up herdsmans
  LOCK_DOWN,   //! Sem for going down herdsmans
  UP_QUEUE,    //! Sem for locking herdsman if queue after bottom enter in bom
  UP,          //! Sem for locking bottom enter in bom
  DOWN         //! Sem for locking top enter in bom
};


//! Semaphors operation: p(c, 15) <=> minus 15 to semaphor c,
//!                      v(c, 15) <=> plus 15 to semaphor c,
//!                      z waiting for NULL


void P_oper(int sem_id, enum Semaphors sem_num, short int num)
{
    short int neg_num = -num;
    struct sembuf sem_struct = {sem_num, (short int)neg_num, 0};

    if (semop(sem_id, &sem_struct, 1) < 0)
    {
        perror("sem op return not zero in P_oper!\n");
        exit(0);
    }
}

void V_oper(int sem_id, enum Semaphors sem_num, short int num)
{
    struct sembuf sem_struct = {sem_num, (short int)num, 0};

    if (semop(sem_id, &sem_struct, 1) < 0)
    {
        perror("sem op return not zero in V_oper!\n");
        exit(0);
    }
}


/*
void Z_oper(int sem_id, enum Semaphors sem_num)
{
    struct sembuf sem_struct = {sem_num, 0, 0};

    if (semop(sem_id, &sem_struct, 1) < 0)
    {
        perror("sem op return not zero in Z_oper!\n");
        exit(0);
    }
}
*/
