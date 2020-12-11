#include "semaphors.h"


void Start_working()
{
  V_oper(sem_id, CHANNEL, 1);

  while (true)
  {
    //! Server take control under the channer!
    P_oper(sem_id, MEMORY, 1);

    printf("%s", main_buffer);

    V_oper(sem_id, CHANNEL, 1);
    Z_oper(sem_id, CHANNEL);
  }
}


//! Server exist for getting data from client and writing it to another terminal

int main()
{
  //! Init block of main
  char buf[BUFFER_SIZE];

  key_t key = ftok(NAME_OF_SERVER, SERVER_ID);

  shm_id = shmget(key, BUFFER_SIZE, 0777 | IPC_CREAT);

  sem_id = semget(key, num_of_semaphors, 0777 | IPC_CREAT);

  main_buffer = shmat(shm_id, NULL, 0);

  setvbuf(stdout, buf, _IOLBF, BUFFER_SIZE);

  Start_working();

  //! Delete segment of shared memory
  shmctl(shm_id, IPC_RMID, NULL);
  //! Delete num of semaphors from system
  semctl(sem_id, 0, IPC_RMID);

  return 0;
}
