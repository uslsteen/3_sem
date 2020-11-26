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
  P_oper(LOCK_UP);
  Upwait++;

  if (Upwait != 1)
  {
    //! Herdsman not first in queue
    V_oper(LOCK_UP);
    P_oper(UP_QUEUE);
    P_oper(LOCK_UP);
  }
  if (!Cap)
  {
    V_oper(LOCK_UP);
    //! Оставляем стадо и идем вверх;
    //! Кладем шапку;
    P_oper(LOCK_UP);
    Cap = 1;
    V_oper(LOCK_UP);
    P_oper(DOWN);
    //! Идем обратно;
    //! ждем возможности входа
    P_oper(UP);
    P_oper(LOCK_UP);
  }

  // формирование колонны
  Upwait--;
  if (Upwait != 0)
  {
    V_oper(UP_QUEUE);
    Upgo++;
    V_oper(LOCK_UP);
  }

  printf("I'm herdsman number %d and I'm going through the bom to the top!\n");

  P_oper(LOCK_UP);
  Upgo--;

  if (Upgo == 0)
  { // конец прохода колонны
    Cap = 0;
    V_oper(DOWN);
  }

  V_oper(LOCK_UP);
}


//! Herdsman at the top !//
void Herdsman_down(int sem_id, int index)
{
  // Проверяем, не лежит ли шапка
  P_oper(DOWN);
  P_oper(LOCK_DOWN);

  // Первый в колонне блокирует вход снизу

  if (Downgo == 0)
  {
    P_oper(UP);
    Downgo ++;
    V_oper(LOCK_DOWN);
    V_oper(DOWN);
  }

  printf("I'm herdsman number %d and I'm going through the bom to the bottom!\n");

  P_oper(LOCK_DOWN);
  Downgo--;

  // Последний в колонне разблокирует вход снизу

  if (Downgo == 0)
  {
    V_oper(UP);
    V_oper(LOCK_DOWN);
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
