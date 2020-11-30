#include "msg.h"




void Herdsman(int sem_id, int index, int num_of_herdmans)
{
    struct Message_t msg;
    printf("Hello, I'm herdsman number %d.\nAnd I'm going to the top of mountain!\n\n", index);
    Herdsman_at_the_bot(sem_id, index, &msg, num_of_herdmans);

    printf("Hello, I'm herdsman number %d.\nAnd I'm going to the top of mountain!\n\n", index);
    Herdsman_at_the_top(sem_id, index, &msg, num_of_herdmans);

    printf("I'm herdsman number %d and I'm going away!\n\n", index);
}


int main(int argc, char** argv)
{
    int num_of_herdmans = 0;

    if (argc == 1)
        num_of_herdmans = 16;


    num_of_herdmans = strtol(argv[1], NULL, 10);

    //Creating new queue
    int id = msgget(IPC_PRIVATE, 0777);

    for (int i = 0; i < num_of_herdmans; ++i)
    {
        //! Creating herdsmans
        pid_t herdmans_pid = fork();

        if (herdmans_pid == 0)
        {
            Herdsman(id, i + 1, num_of_herdmans);
            return 0;
        }

        else if (herdmans_pid < 0)
        {
            perror("fork return negative val to herdsman pid!\n");
            return ERROR;
        }
    }

    return 0;
}