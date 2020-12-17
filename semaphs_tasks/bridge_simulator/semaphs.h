#ifndef __SEMAPHORS_H__
#define __SEMAPHORS_H__

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>


const int num_of_semaphors = 4;

const int MAX_PERMISSION  = 0777;
const int BUFFER_SIZE     = 4096;

//! Vars for working with shared memory and semaphors
int sem_id = 0;
int shm_id = 0;

int num_of_waiting_cars = 0;
int num_o_waiting_ships = 0;



//char* main_buffer = NULL;


enum Semaphors
{
  SHIP,
  CAR,
  BRIDGE
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

#endif // __SEMAPHORS_H__
