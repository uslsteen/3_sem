#include "semaphs.h"



void Car_method(int sem_id, int index, struct Bridge_organization* shm_struct)
{
  printf("Hello, I'm driver[%d]\n\n", index);

  //! Driver[index] went to the begin of the bridge
  P_oper(sem_id, SHRD_VAR, 1);

  (shm_struct->num_of_waiting_cars)++;
  printf("At the moment driver[%d] stay in queue!\n\n", index);

  V_oper(sem_id, SHRD_VAR, 1);

  printf("Driver[%d] gonna drive through the bridge!\n\n", index);

  P_oper(sem_id, CAR, 1);
  P_oper(sem_id, BRIDGE, 1);

  printf("And now driver [%d] on the bridge!\n\n", index);


  P_oper(sem_id, SHRD_VAR, 1);

  (shm_struct->num_of_waiting_cars)--;
  printf("Driver[%d] drove the bridge!\n\n", index);

  V_oper(sem_id, SHRD_VAR, 1);

  P_oper(sem_id, SHRD_VAR, 1);

  //! There is checking for possibility moving of cars or ships
  if (( shm_struct->num_of_waiting_ships != 0 && shm_struct->num_of_waiting_cars == 0 ) || ( shm_struct->num_of_waiting_ships > 2 ))
  {
    //! Ship should move
    V_oper(sem_id, SHIP, 1);

    if (shm_struct->bridge_cond == 0)
    {
      printf("The employees of the transport system in a hurry raise the bridge!\n\n");
      shm_struct->bridge_cond = 1;

      printf("Success, employees raised the bridge!\n\n");
    }
  }
  else
  {
    //! Cars should move
    V_oper(sem_id, CAR, 1);

    if (shm_struct->bridge_cond == 1)
    {
      printf("Employees of the transport system in a hurry lower the bridge.\n");
      shm_struct->bridge_cond = 0;

      printf("Success, employees lowed the bridge!\n\n");
    }
  }

  V_oper(sem_id, SHRD_VAR, 1);
  V_oper(sem_id, BRIDGE, 1);
}




void Ship_method(int sem_id, int index, struct Bridge_organization* shm_struct)
{
  printf("Hello, I'm captain of the ship[%d]\n", index);

  //! Driver[index] went to the begin of the bridge
  P_oper(sem_id, SHRD_VAR, 1);

  shm_struct->num_of_waiting_ships++;
  printf("At the moment captain of the ship[%d] stay in queue!\n\n", index);

  V_oper(sem_id, SHRD_VAR, 1);

  printf("Captain of the ship[%d] gonna swim under the bridge!\n\n", index);

  //! Ship start moving
  V_oper(sem_id, SHIP, 1);
  P_oper(sem_id, BRIDGE, 1);

  printf("And now captain of the ship[%d] under the bridge!\n\n", index);

  P_oper(sem_id, SHRD_VAR, 1);

  shm_struct->num_of_waiting_ships--;
  printf("Captain of the ship[%d] swome under the bridge!\n\n", index);

  V_oper(sem_id, SHRD_VAR, 1);

  P_oper(sem_id, SHRD_VAR, 1);

  //! There is checking for possibility moving of cars or ships
  if (shm_struct->num_of_waiting_ships > 2 || ( (shm_struct->num_of_waiting_cars == 0) && (shm_struct->num_of_waiting_ships != 0) ) )
  {
    //! Ship should move
    V_oper(sem_id, SHIP, 1);

    if (shm_struct->bridge_cond == 0)
    {
      printf("The employees of the transport system in a hurry raise the bridge!\n\n");
      shm_struct->bridge_cond = 1;

      printf("Success, employees raised the bridge!\n\n");
    }
  }
  else
  {
    //! Cars should move
    V_oper(sem_id, CAR, 1);

    if (shm_struct->bridge_cond == 1)
    {
      printf("Employees of the transport system in a hurry lower the bridge.\n");
      shm_struct->bridge_cond = 0;

      printf("Success, employees lowed the bridge!\n\n");
    }
  }

  V_oper(sem_id, SHRD_VAR, 1);
  V_oper(sem_id, BRIDGE, 1);
}




void Semaphs_init(int sem_id)
{
  V_oper(sem_id, SHRD_VAR, 1);
  V_oper(sem_id, CAR, 1);
  V_oper(sem_id, BRIDGE, 1);
}

int main(int argc, char** argv)
{
  int num_of_ships = 5, num_of_cars = 10, sem_id = 0, shm_id = 0;

  if (argc == 3)
  {
    num_of_ships = atoi(argv[1]);
    num_of_cars  = atoi(argv[2]);
  }

  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);

  pid_t ship_pid = 0, car_pid = 0, pid = 0;

  if ((sem_id = semget(IPC_PRIVATE, num_of_semaphors, MAX_PERMISSION)) < 0)
    return Err_proc("semget return negative value!\n");


  if ((shm_id = shmget(IPC_PRIVATE, BUFSIZ, MAX_PERMISSION | IPC_PRIVATE)) < 0)
    return Err_proc("shmget return negative value!\n");

  struct Bridge_organization* shm_struct = (struct Bridge_organization*) calloc(1, sizeof(struct Bridge_organization));
  shm_struct = (struct Bridge_organization*) shmat(shm_id, NULL, 0);

  shm_struct->num_of_waiting_cars = 0;
  shm_struct->num_of_waiting_ships = 0;
  shm_struct->bridge_cond = 1;

  Semaphs_init(sem_id);

  for (int index = 0; index < num_of_cars + num_of_ships; ++index)
  {
    /*
    if ((ship_pid = fork()) < 0)
      return Err_proc("fork return neg value to ship pid!\n");

    if ((car_pid = fork()) < 0)
      return Err_proc("fork return neg value to ship pid!\n");
    */


    if ((pid = fork()) < 0)
        return Err_proc("fork return neg value to pid\n");

    if (!pid)
    {
        if (index < num_of_ships)
            Ship_method(sem_id, (index + 1), shm_struct);

        else Car_method(sem_id, (index - num_of_ships + 1), shm_struct);

        return 0;
    }
  }

  for (int index = 1; index < num_of_cars + num_of_ships; ++index);
  {
    if (wait(NULL) < 0)
      return Err_proc("wait return negative value!\n");

  }


  if (semctl(sem_id, num_of_semaphors, IPC_RMID) < 0)
    return Err_proc("semctl return negative value!\n");


  if (shmctl(shm_id, IPC_RMID, (struct shmid_ds *)shm_struct ) < 0)
    return Err_proc("shmctl return negative value!\n");


  return 0;
}


























/*

*/
