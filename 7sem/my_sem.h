#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


//* const int num_of_people = 10;
//* const int cap_of_boat = 2;
//* const int num_of_travels = 5;
const int num_of_semaphors = 10;

const int ERROR = -1;


//! ?????????????????????????????????????????????????? !//
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};
//! ?????????????????????????????????????????????????? !//


enum Semaphors
{

};

struct sem_buf
{
  unsigned short sem_num;  //! semaphore number
  short          sem_op;   //! semaphore operation
  short          sem_flg;  //! operation flags (it will be zero in my realization)

}

//! Semaphors operation: p(c, 15) <=> minus 15 to semaphor c,
//!                      v(c, 15) <=> plus 15 to semaphor c,
//!                      z


void P_oper(int sem_id, enum Semaphors sem_num, int num)
{
  struct sem_buf sem_struct = {sem_num, -num, 0};

  if (semop(sem_id, sem_struct, 1) != 0)
  {
    perror("sem op return not zero in P_oper!\n");
    exit(0);
  }
}

void V_oper(int sem_id, enum Semaphors sem_num, int num)
{
  struct sem_buf sem_struct = {sem_num, num, 0};

  if (semop(sem_id, sem_struct, 1) != 0)
  {
    perror("sem op return not zero in V_oper!\n");
    exit(0);
  }
}

void Z_oper(int sem_id, enum Semaphors sem_num)
{
  struct sem_buf sem_struct = {sem_num, 0, 0};

  if (semop(sem_id, sem_struct, 1) != 0)
  {
    perror("sem op return not zero in Z_oper!\n");
    exit(0);
  }
}
