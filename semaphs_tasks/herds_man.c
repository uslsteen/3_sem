#include "semaphors.h"

void Herdsman(int sem_id, int index)
{
  printf("Hello, I'm herdsman number %.\nAnd I'm going to the top of mountain!\n");
  Herdsman_down(sem_id, index)

  printf("Hello, I'm herdsman number %.\nAnd I'm going to the top of mountain!\n");
  Herdsman_up(sem_id, index)
}

//! Herdsman in the bottom !//
void Herdsman_down(int sem_id, int index)
{
  P_oper(Lock_up);
  Upwait++;
  if (Upwait != 1)
  {
    //! Herdsman not first in queue
    V_oper(Lock_up);
    P_oper(Up_queue);
    (Lock_up);
  }
  if (!Cap)
  {
    V_oper(Lock_up);

    //! Оставляем стадо и идем вверх;
    //! Кладем шапку;
    P_oper(Lock_up);
    Cap = 1;
    V_oper(Lock_up);
    P_oper(Down);
    //! Идем обратно;
    //! ждем возможности входа
    P_oper(Up);
    P_oper(Lock_up);
  }

  // формирование колонны
  Upwait--;
  if (Upwait != 0)
  {
    V_oper(Up_queue);
    Upgo++;
    V_oper(Lock_up);
  }

  printf("I'm herdsman number %d and I'm going through the bom to the top!\n");

  P_oper(Lock_up);
  Upgo--;

  if (Upgo == 0)
  { // конец прохода колонны
    Cap = 0;
    V_oper(Down);
  }

  V_oper(Lock_up);
}


//! Herdsman at the top !//
void Herdsman_down(int sem_id, int index)
{
  // Проверяем, не лежит ли шапка
  P_oper(Down);
  P_oper(Lock_down);

  // Первый в колонне блокирует вход снизу

  if (Downgo == 0)
  {
    P_oper(Up);
    Downgo ++;
    V_oper(Lock_down);
    V_oper(Down);
  }

  printf("I'm herdsman number %d and I'm going through the bom to the bottom!\n");

  P_oper(Lock_down);
  Downgo--;

  // Последний в колонне разблокирует вход снизу

  if (Downgo == 0)
  {
    V_oper(Up);
    V_oper(Lock_down);
  }
}

void Sems_processing(int sem_id, int num_of_herdmans)
{
    //! Semaphors initialization
    //! Semaphore Lock_up = 1, Lock_down = 1;
    //! Semaphore Up_queue = 0, Up = 1, Down = 1;

    V_oper(sem_id, LOCK_UP, 1);
    V_oper(sem_id, LOCK_DOWN, 1);
    V_oper(sem_id, UP_QUEUE);
    V_oper(sem_id, UP, 1);
    V_oper(sem_id, DOWN, 1);
}

int main()
{
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
  pid_t herdmans_pid = 0;

  int num_of_herdmans = 5;

  int sem_i= semget(IPC_PRIVATE, num_of_semaphors, 0777);

  if (sem_id < 0)
  {
    perror("Semget return error!\n");
    return ERROR;
  }

  Sems_processing(sem_id, num_of_herdmans);

  if ((herdmans_pid = fork()) < 0)
  {
    perror("Fork return negative pid!\n");
    return ERROR;
  }

  if (herdmans_pid == 0)
  {
    for (int i = 0; i < num_of_herdmans; ++i)
    {
      Herdsman(sem_id, i);
      return 0;
    }
  }

  return 0;
}
