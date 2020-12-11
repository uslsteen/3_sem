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


const int num_of_semaphors = 0;

const int SERVER_ID = 0;
const char NAME_OF_SERVER[] = "servec.c"


//! Vars for working with shared memory and semaphors
int semaphs_id = 0;
int shrd_mem_id = 0;
char* shrd_mem_buf = NULL;

enum Semaphors
{
  CHANNEL,
  MEMORY
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
    struct sembuf sem_struct = {sem_name, (short int)-num, 0};

    if (semop(sem_id, &sem_struct, 1) < 0)
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


/*
void Z_oper(int sem_id, enum Semaphors sem_name)
{
    struct sembuf sem_struct = {sem_name, 0, 0};
    if (semop(sem_id, &sem_struct, 1) < 0)
    {
        perror("sem op return not zero in Z_oper!\n");
        exit(0);
    }
}
*/
