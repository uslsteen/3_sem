 #include "semaphors.h"



//! Herdsman in the bottom !//
void Herdsman_down(int sem_id, int index)
{
  P_oper(sem_id, LOCK_UP, 1);
  Upwait++;

  if (Upwait != 1)
  {
    //! Herdsman not first in queue
    V_oper(sem_id, LOCK_UP, 1);
    P_oper(sem_id, UP_QUEUE, 1);
    P_oper(sem_id, LOCK_UP, 1);
  }
  if (!Cap)
  {
    V_oper(sem_id, LOCK_UP, 1);
    //! Оставляем стадо и идем вверх;
    //! Кладем шапку;
    P_oper(sem_id, LOCK_UP, 1);
    Cap = 1;
    V_oper(sem_id, LOCK_UP, 1);
    P_oper(sem_id, DOWN, 1);

    //! Идем обратно;
    //! ждем возможности входа
    P_oper(sem_id, UP, 1);
    P_oper(sem_id, LOCK_UP, 1);
  }

  // формирование колонны
  Upwait--;
  if (Upwait != 0)
  {
    V_oper(sem_id, UP_QUEUE, 1);
    Upgo++;
    V_oper(sem_id, LOCK_UP, 1);
  }

  printf("I'm herdsman number %d and I'm going through the bom to the top!\n", index);

  P_oper(sem_id, LOCK_UP, 1);
  Upgo--;

  if (Upgo == 0)
  { // конец прохода колонны
    Cap = 0;
    V_oper(sem_id, DOWN, 1);
    printf("I'm herdsman number %d , and I went through the bom!\n", index);
  }

  V_oper(sem_id, LOCK_UP, 1);
}


//! Herdsman at the top !//
void Herdsman_up(int sem_id, int index)
{
  // Проверяем, не лежит ли шапка
  P_oper(sem_id, DOWN, 1);
  P_oper(sem_id, LOCK_DOWN, 1);

  // Первый в колонне блокирует вход снизу

  if (Downgo == 0)
  {
    P_oper(sem_id, UP, 1);
    Downgo ++;
    V_oper(sem_id, LOCK_DOWN, 1);
    V_oper(sem_id, DOWN, 1);
  }

  printf("I'm herdsman number %d and I'm going through the bom to the bottom!\n", index);

  P_oper(sem_id, LOCK_DOWN, 1);
  Downgo--;

  // Последний в колонне разблокирует вход снизу

  if (Downgo == 0)
  {
    V_oper(sem_id, UP, 1);
    V_oper(sem_id, LOCK_DOWN, 1);
  }
}

void Sems_processing(int sem_id)
{
    //! Semaphors initialization
    //! Semaphore Lock_up = 1, Lock_down = 1;
    //! Semaphore Up_queue = 0, Up = 1, Down = 1;
    V_oper(sem_id, LOCK_UP, 1);
    V_oper(sem_id, LOCK_DOWN, 1);
    V_oper(sem_id, UP_QUEUE, 1);
    P_oper(sem_id, UP_QUEUE, 1);
    V_oper(sem_id, UP, 1);
    V_oper(sem_id, DOWN, 1);
}

void Herdsman(int sem_id, int index)
{
  printf("Hello, I'm herdsman number %d.\nAnd I'm going to the top of mountain!\n", index);
  Herdsman_down(sem_id, index);

  printf("Hello, I'm herdsman number %d.\nAnd I'm going to the top of mountain!\n", index);
  Herdsman_up(sem_id, index);

  printf("I'm herdsman number %d and I'm going away!\n", index);
}

int main(int argc, char** argv)
{
  pid_t herdmans_pid = 0;
  int num_of_herdmans = 0;

  if (argc < 2)
    num_of_herdmans = 5;
  else
    num_of_herdmans = atoi(argv[1]);

  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);

  int sem_id = semget(IPC_PRIVATE, num_of_semaphors, 0777);

  if (sem_id < 0)
  {
    perror("Semget return error!\n");
    return ERROR;
  }

  Sems_processing(sem_id);

  if ((herdmans_pid = fork()) < 0)
  {
    perror("Fork return negative pid!\n");
    return ERROR;
  }

  if (herdmans_pid == 0)
  {
    for (int i = 1; i != num_of_herdmans; ++i)
    {
      Herdsman(sem_id, i);
      return 0;
    }
  }

  for (int i = 0; i < num_of_herdmans; ++i)
  {
    if (wait(NULL) < 0)
    {
      perror("wait return error!\n");
      return ERROR;
    }
  }

  semctl(sem_id, 0, IPC_RMID);


  return 0;
}
