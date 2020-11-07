#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>


const int ERROR = -1;

unsigned long long counter = 0;
pthread_mutex_t pthr_mut = PTHREAD_MUTEX_INITIALIZER;

typedef *(*func_t)(void*);

void Print_time(struct timespec start_clock, struct timespec final_clock)
{
  double secs = final_clock.tv_sec - start_clock.tv_sec;
  double nsecs = final_clock.tv_nsec - start_clock.tv_nsec;

  double time = secs*1000 + nsecs/1000000;

  printf("Process time in msecs : %lf\n", time);
}


void* First_algo(void* param)
{
  int prm = *(int*)param;

  for (int i = 0; i < prm; ++i)
    counter++;

  return 0;
}


void* Second_algo(void* param)
{
  int prm = *(int*)param;

  for (int i = 0; i < prm; ++i)
  {
    pthread_mutex_lock(&pthr_mut);
    counter++;
    pthread_mutex_unlock(&pthr_mut);
  }

  return 0;
}

void* Third_algo(void* param)
{
  int prm = *(int*)param;
  pthread_mutex_lock(&pthr_mut);

  for (int i = 0; i < prm; ++i)
    counter++;

  pthread_mutex_unlock(&pthr_mut);

  return 0;
}

void* Fourth_algo(void* param)
{
  int prm = *(int*)param;
  int loc_counter = 0;

  for (int i = 0; i < prm; ++i)
    loc_counter++;

  pthread_mutex_lock(&pthr_mut);
  counter += loc_counter;
  pthread_mutex_unlock(&pthr_mut);

  return 0;
}

void My_threads(unsigned long long num_of_threads, unsigned long long expctd_val, pthread_t* thr_buf, func_t func)
{
  struct timespec start_clock = {0, 0}, final_clock = {0, 0};
  int value_per_threads = expctd_val/num_of_threads;
  int sec_param = value_per_threads + expctd_val % num_of_threads;

  if (clock_gettime(CLOCK_REALTIME, &start_clock) != 0)
  {
    perror("Eror in clock_gettime, struct start_clock!\n");
    exit(0);
  }

  pthread_create(thr_buf, NULL, func, &sec_param);

  for (size_t i = 1; i < num_of_threads; ++i)
    pthread_create(thr_buf + i, NULL, func, &value_per_threads);

  for (size_t i = 0; i < num_of_threads; ++i)
    pthread_join(thr_buf[i], NULL);

  if (clock_gettime(CLOCK_REALTIME, &final_clock) != 0)
  {
    perror("Eror in clock_gettime, struct final clock!\n");
    exit(0);
  }

  Print_time(start_clock, final_clock);

  printf("I extectep %llu, and I got %llu\n", expctd_val, counter);
}


int main(int argc, char** argv)
{
  if (argc < 4)
  {
    perror("Lack of arguments!\n");
    return ERROR;
  }

  unsigned long long num_of_threads = atoi(argv[1]);
  unsigned long long num_of_algo    = atoi(argv[2]);
  unsigned long long expctd_val     = atoi(argv[3]);

  func_t functions[4] = {First_algo, Second_algo, Third_algo, Fourth_algo};

  pthread_t thr_buf[num_of_threads];

  pthread_mutex_unlock(&pthr_mut);

  My_threads(num_of_threads, expctd_val, thr_buf, functions[num_of_algo - 1]);

  return 0;
}
