#include "semaphs.h"


void Car_method(int sem_id, int index)
{
  printf("Hello, I'm driver number %d", index);

  //! Driver[index] went to the begin of the bridge
  P_oper(sem_id, SHRD_VAR, 1);

  num_of_waiting_cars++;
  printf("At the moment driver number %d stay in queue!\n", index);

  V_oper(sem_id, SHRD_VAR, 1);

  printf("I'm gonna drive through the bridge!\n");

  P_oper(sem_id, CAR, 1);
  P_oper(sem_id, BRIDGE, 1);

  printf("And now driver nuber %d on the bridge!\n");


  P_oper(sem_id, SHRD_VAR, 1);

  num_of_waiting_cars--;
  printf("Driver number %d  drove the bridge!\n", index);

  V_oper(sem_id, SHRD_VAR, 1);

  P_oper(sem_id, SHRD_VAR, 1);

  //! There is checking for possibility moving of cars or ships
  if ((num_o_waiting_ships != 0 && num_of_waiting_cars == 0) ||(num_o_waiting_ships > 2))
  {
    //! Ship should move
    //P_oper(BOAT);
    V_oper(sem_id, SHIP, 1);

    if (bridge_cond)
    {
      printf("The employees of the transport system in a hurry raise the bridge!\n");
      bridge_cond = 0;

      printf("Success, employees raised the bridge!\n");
    }
  }
  else
  {
    //! Cars should move
    V_oper(sem_id, CAR, 1);

    if (!bridge_cond)
    {
      printf("Employees of the transport system in a hurry lower the bridge.");
      bridge_cond = 0;

      printf("Success, employees lowed the bridge!\n");
    }
  }

  V_oper(sem_id, SHRD_VAR, 1);
  V_oper(sem_id, BRIDGE, 1);
}





void Ship_method(int sem_id, int index)
{
  printf("Hello, I'm captain of the ship number %d", index);

  //! Driver[index] went to the begin of the bridge
  P_oper(sem_id, SHRD_VAR, 1);

  num_of_waiting_ships++;
  printf("At the moment captain of the ship number[%d] stay in queue!\n", index);

  V_oper(sem_id, SHRD_VAR, 1);

  printf("Captain[%d] gonna swim under the bridge!\n", index);

  //! Ship start moving
  V_oper(sem_id, SHIP, 1);
  P_oper(sem_id, BRIDGE, 1);

  printf("And now captain[%d] under the bridge!\n");

  P_oper(sem_id, SHRD_VAR, 1);

  num_of_waiting_ships--;
  printf("Driver number %d  drove the bridge!\n", index);
  V_oper(sem_id, SHRD_VAR, 1);

  //!

  P_oper(sem_id, SHRD_VAR, 1);
  //! There is checking for possibility moving of cars or ships
  if (num_o_waiting_ships != 0)
  {
    //! Ship should move
    //P_oper(BOAT);
    V_oper(sem_id, BOAT, 1);

    if (bridge_cond)
    {
      printf("The employees of the transport system in a hurry raise the bridge!\n");
      bridge_cond = 0;

      printf("Success, employees raised the bridge!\n");
    }
  }
  else
  {
    //! Cars should move
    V_oper(sem_id, CAR, 1);

    if (!bridge_cond)
    {
      printf("Employees of the transport system in a hurry lower the bridge.");
      bridge_cond = 0;

      printf("Success, employees lowed the bridge!\n");
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
  int num_of_ships = 5, num_of_cars = 10;

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

/*
  if ((shm_id = shmget(IPC_PRIVATE, BUFSIZ, MAX_PERMISSION | IPC_PRIVATE)) < 0)
    return Err_proc("shmget return negative value!\n");
*/

  Semaphs_init(sem_id);

  for (int index = 0; index < num_of_cars + num_of_ships; ++i)
  {

    if ((ship_pid = fork()) < 0)
      return Err_proc("fork return neg value to ship pid!\n");

    if ((car_pid = fork()) < 0)
      return Err_proc("fork return neg value to ship pid!\n");

    if (i < num_of_cars)
      Car_method(index, sem_id);

    else Ship_method(num_of_cars - index, sem_id);

    return 0;
  }

  for (int i = 0; i < num_of_cars + num_of_ships; ++i);
  {
    if (wait(NULL) < 0)
      return Err_proc("wait return negative value!\n");
  }

  if (semctl(sem_id, ..., IPC_RMID) < 0)
    return Err_proc("semctl return negative value!\n");

/*
  if (shmctl(shm_id, ..., IPC_RMID) < 0)
    return Err_proc("shmctl return negative value!\n");
*/

  return 0;
}


























/*
if ((pid = fork()) < 0)
  return Err_proc("fork return neg value to pid\n");
*/
