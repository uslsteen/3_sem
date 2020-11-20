
void Herdsman_down()
{
  P(Lock_up); Upwait++;
  if (Upwait != 1){
  // не первый в очереди
  V(Lock_up); P(Up_queue); P(Lock_up);
  }
  if (!Cap) {
  V(Lock_up);
  Оставляем стадо и идем вверх;
  Кладем шапку;
  P(Lock_up); Cap = 1; V(Lock_up);  P(Down);
  Идем обратно;
  // ждем возможности входа
  P(Up); P(Lock_up);
  }

  // формирование колонны

  Upwait--;
  if (Upwait != 0) V(Up_queue);
  Upgo++; V(Lock_up);

  { проход по бому; }

  P(Lock_up); Upgo--;
  if (Upgo == 0) { // конец прохода колонны
  Cap = 0; V(Down);
  }
  V(Lock_up);
}


void Herdsman_down()
{
  // Проверяем, не лежит ли шапка
  P(Down);
  P(Lock_down);

  // Первый в колонне блокирует вход снизу

  if (Downgo == 0) P(Up);
  Downgo ++;
  V(Lock_down); V(Down);

  { проход по бому; }

  P(Lock_down);
  Downgo--;

  // Последний в колонне разблокирует вход снизу

  if (Downgo == 0)V(Up);
  V(Lock_down);
}

int main()
{
  

  //! Semaphors initialization
  //! Semaphore Lock_up = 1, Lock_down = 1;
  //! Semaphore Up_queue = 0, Up = 1, Down = 1;
}
