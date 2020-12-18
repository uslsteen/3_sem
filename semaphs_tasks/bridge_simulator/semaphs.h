//
// Created by anon on 18.12.2020.
//
#ifndef BRIDGE_SEMS_H
#define BRIDGE_SEMS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


//Shared structure
struct Bridge_organization
{
    unsigned num_of_waiting_cars;
    unsigned num_of_waiting_ships;
    int bridge_cond;
};


const int num_of_semaphors = 4;

const int MAX_PERMISSION  = 0777;
const int BUFFER_SIZE     = 4096;
const int SHM_SIZE = 4096;

const unsigned int MAX_SHIPS = 3;

const int OPEN = 1;
const int CLOSE = 0;


enum Semaphors
{
    CAR,
    SHIP,
    BRIDGE,
    SHRD_VAR
};


int Err_proc(char* err_string)
{
    perror(err_string);
    return errno;
}


//! Semaphors operation: p(c, 15) <=> minus 15 to semaphor c,
//!                      v(c, 15) <=> plus 15 to semaphor c,
//!                      z waiting for NULL


void P_oper(int sem_id, enum Semaphors sem_name, short int num)
{
    struct sembuf sem_struct[] = {{sem_name, (short int)-num, 0}};

    if (semop(sem_id, sem_struct, 1) < 0)
    {
        perror("sem op return not zero in P_oper!\n");
        exit(0);
    }
}

void V_oper(int sem_id, enum Semaphors sem_name, short int num)
{
    struct sembuf sem_struct = {sem_name, (short int)num, 0};

    if (semop(sem_id, &sem_struct, 1) < 0)
    {
        perror("sem op return not zero in V_oper!\n");
        exit(0);
    }
}

void Z_oper(int sem_id, enum Semaphors sem_name)
{
    struct sembuf sem_struct = {sem_name, 0, 0};
    if (semop(sem_id, &sem_struct, 1) < 0)
    {
        perror("sem op return not zero in Z_oper!\n");
        exit(0);
    }
}

#endif //BRIDGE_SEMS_H
