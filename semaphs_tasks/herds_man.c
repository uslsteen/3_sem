

void Herdsman(int sem_id, int index)
{
  printf("Hello, I'm herdsman number %.\nAnd I'm going to bom!\n");


}

//! Herdsman in the bottom !//
void Herdsman_down(int sem_id)
{
  P(Lock_up);
  Upwait++;
  if (Upwait != 1)
  {
    // не первый в очереди
    V(Lock_up);
    P(Up_queue);
    (Lock_up);
  }
  if (!Cap)
  {
    V(Lock_up);
    //! Оставляем стадо и идем вверх;
    //! Кладем шапку;
    P(Lock_up);
    Cap = 1;
    V(Lock_up);
    P(Down);
    //! Идем обратно;
    //! ждем возможности входа
    P(Up);
    P(Lock_up);
  }

  // формирование колонны
  Upwait--;
  if (Upwait != 0)
  {
    V(Up_queue);
    Upgo++;
    V(Lock_up);
  }

  { проход по бому; }

  P(Lock_up);
  Upgo--;

  if (Upgo == 0)
  { // конец прохода колонны
    Cap = 0;
    V(Down);
  }

  V(Lock_up);
}


//! Herdsman at the top !//
void Herdsman_down()
{
  // Проверяем, не лежит ли шапка
  P(Down);
  P(Lock_down);

  // Первый в колонне блокирует вход снизу

  if (Downgo == 0)
  {
    P(Up);
    Downgo ++;
    V(Lock_down);
    V(Down);
  }

  { проход по бому; }

  P(Lock_down);
  Downgo--;

  // Последний в колонне разблокирует вход снизу

  if (Downgo == 0)
  {
    V(Up);
    V(Lock_down);
  }
}

void Sems_processing(int sem_id, int num_of_herdmans)
{
    //! Semaphors initialization
    V_oper(sem_id, LOCK_UP, 1);
    V_oper(sem_id, LOCK_DOWN, 1);
    V_oper(sem_id, UP_QUEUE);
    V_oper(sem_id, UP, 1);
    V_oper(sem_id, DOWN, 1);
}

int main()
{

  //! Semaphore Lock_up = 1, Lock_down = 1;
  //! Semaphore Up_queue = 0, Up = 1, Down = 1;

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
