#include "semaphs.h"

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

  for (int i = 0; i < num_of_cars + num_of_ships; ++i)
  {
    /*
    if ((ship_pid = fork()) < 0)
      return Err_proc("fork return neg value to ship pid!\n");

    if ((car_pid = fork()) < 0)
      return Err_proc("fork return neg value to ship pid!\n");
    */

    if ((pid = fork()) < 0)
      return Err_proc("fork return neg value to pid\n");

    if (i < num_of_cars)
      Car_method();

    else Ship_method();

    return 0;
  }

  for (int i = 0; i < num_of_cars + num_of_ships; ++i);
  {
    if (wait(NULL) < 0)
      return Err_proc("wait return negative value!\n");
  }

  if (semctl(sem_id, ..., IPC_RMID) < 0)
    return Err_proc("semctl return negative value!\n");


  return 0;
}