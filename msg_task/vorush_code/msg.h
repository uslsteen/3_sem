//
// Created by anon on 29.11.2020.
//

#ifndef VORUSH_CODE_SEMS_H
#define VORUSH_CODE_SEMS_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>


#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/msg.h>


const int ERROR = -1;

#define RUNNR_CAME (num_of_runnrs + 2)
#define RUN_IS_OVR (num_of_runnrs + 1)

int Upwait = 0;  //! Variable for num of waiting herdsmans
int Up_go = 0;    //! Var for num of going up herdsmans
int Down_go = 0;  //! Var for num of going down herdsmans
int Cap = 0;         //! Flag for hat on the top enter


enum Semaphors
{
    LOCK_UP = 0, //! Sem for going up herdsmans
    LOCK_DOWN,   //! Sem for going down herdsmans
    UP_QUEUE,    //! Sem for locking herdsman if queue after bottom enter in bom
    UP,          //! Sem for locking bottom enter in bom
    DOWN         //! Sem for locking top enter in bom
};

struct Message_t
{
    long value;
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


#endif //VORUSH_CODE_SEMS_H
