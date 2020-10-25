#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define RUNNR_CAME (num_of_runnrs + 2)

#define RUN_IS_OVR (num_of_runnrs + 1)

const int ERROR = -1;

struct Message_t
{
  long value;
};


//Judge and runners doesnt create queue

void Judge(int id, int num_of_runnrs)
{
  printf("Hello, I'm judge!\n");

  struct Message_t msg;
  int msg_sze = sizeof(msg);

  //! Waiting for runners
  for (int i = 0; i < num_of_runnrs; ++i)
  {
    if (msgrcv(id, &msg, msg_sze, RUNNR_CAME, 0) < 0)
    {
      perror("msgrcv return negative value\n");
      exit(0);
    }
  }

  //! Start competition
  printf("Competition is starting!\n");

  for (int i = 0; i < num_of_runnrs; ++i)
  {
    msg.value = i + 1;

    //! (i + 1) should runner start competition
    //! Judge get info to i + 1 runner
    if (msgsnd(id, &msg, msg_sze, 0) < 0)
    {
      perror("msgsnd return negative value\n");
      exit(0);
    }

    //! (i + 1) runner finish competition
    //! Judge should recieve msg from runner (i + 1)
    if (msgrcv(id, &msg, msg_sze, i + 1, 0) < 0)
    {
      perror("msgrcv return negative value\n");
      exit(0);
    }
  }

  //! Judge end the competition
  printf("Let's end our competition\n");

  for (int i = 0; i < num_of_runnrs; ++i)
  {
    msg.value = RUN_IS_OVR;

    if (msgsnd(id, &msg, msg_sze, 0) < 0)
    {
      perror("msgsnd return negative value\n");
      exit(0);
    }
  }
}


void Runner(int id, int cur_runner, int num_of_runnrs)
{
  struct Message_t msg;
  msg.value = RUNNR_CAME;
  int msg_sze = sizeof(msg);

  printf("%d runner has come to start!\n", cur_runner);

  //! Get info to judge
  if (msgsnd(id, &msg, msg_sze, 0))
  {
    perror("msgsnd return negative value\n");
    exit(0);
  }

  //! Current runner is waitinng for command from judge to start
  if (msgrcv(id, &msg, msg_sze, cur_runner, 0) < 0)
  {
    perror("msgrcv return negative value\n");
    exit(0);
  }

  //! Current runner start competition
  printf("%d runner start running!\n", cur_runner);
  printf("%d runned up competition!\n", cur_runner);

  msg.value = cur_runner;

  //Sending info to judge about end of the run
  if (msgsnd(id, &msg, msg_sze, 0) < 0)
  {
    perror("msgsnd return negative value\n");
    exit(0);
  }

  //Getting info for runners about end of competition
  if (msgrcv(id, &msg, msg_sze, RUN_IS_OVR, 0) < 0)
  {
    perror("msgrcv return negative value\n");
    exit(0);
  }

  //! current runner end the competition
  printf("%d run and went away\n", cur_runner);
}


int main(int argc, char** argv)
{
  if (argc == 1)
  {
    perror("Lack of arguments!\n");
    return 0;
  }

  int num_of_runnrs = strtol(argv[1], NULL, 10);

  //Creating new queue
  int id = msgget(IPC_PRIVATE, 0777);

  //Creating judge process
  pid_t judge_pid = fork();

  if (judge_pid == 0)
  {
    Judge(id, num_of_runnrs);

    //When judge will be over, process will be over
    exit(0);
  }

  else if (judge_pid < 0)
  {
    perror("fork return negative val to judge pid!\n");
    return ERROR;
  }

  for (int i = 0; i < num_of_runnrs; ++i)
  {
    //! Creating runners
    pid_t run_pid = fork();

    if (run_pid == 0)
    {
      Runner(id, i + 1, num_of_runnrs);
      return 0;
    }

    else if (run_pid < 0)
    {
      perror("fork return negative val to judge pid!\n");
      return ERROR;
    }
  }

  return 0;
}

//#undef RUNNR_CAME
//#undef RUNNR_IS_OVR
