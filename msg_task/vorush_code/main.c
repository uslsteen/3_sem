#include "msg.h"

#define HERDSMAN_IN_BOM (num_of_herdmans + 2)
#define HERDSMAN_IN_QUEUE (num_of_herdmans + 1)

//! Herdsman in the bottom !//
void Herdsman_down(int id, int index, struct Message_t* msg)
{
    int msg_size = sizeof(*msg);
    //msg->value = HERDSMAN_CAME;
    // msgsnd(id &msg msg_size, 0);
    //P_oper(sem_id, LOCK_UP, 1);
    Up_wait++;

    //! Herdsman not first in queue
    if (Up_wait != 1)
    {
     //   msg->value = HERDSMAN_IN_QUEUE;
        msgrcv(id, msg, msg_size, HERDSMAN_IN_QUEUE, 0);
    }

    if (!Cap)
    {
        //! Оставляем стадо и идем вверх;
        //! Кладем шапку;
        //! Heardsman[index] going to the top of mountain
        printf("Herdsman number %d left his sheeps and went to the top of mountain!\n", index);
        Cap = 1;

        printf("Herdsman number %d put your hat on the ground!\n", index);
        printf("Herdsman number %d went to the down of mountain!\n", index);

        //! Heardsman[index] going back and waiting for possibility enter to the bom
        if (Down_go != 0)
        {
            printf("Herdsman is waiting for free of bom!\n");
            msgrcv(id, msg, msg_size, index, 0);
        }
        //! Herdsman prepare for travel through the bom
    }

    //! Resizing of herdsmans queue
    Up_wait--;
    //! Processing for queue
    //! Here herdsman[index] send message to other heardsmans about decreasing queue
    if (Up_wait != 0)
    {
        msg->value = HEARDSMAN_IN_QUEUE;
        msgsnd(id, msg, msg_size, 0);
        Up_go++;
    }

    //! Herdsman[index] took your hat ang go the land
    Cap = 0;
    //! Herdsman[index] send message to his colleague
    msg->value = HERDSMAN_IN_BOM;
    msgsnd(id, msg, msg_size, 0);
    Up_go--;
    printf("I'm herdsman number %d and I'm going through the bom to the top!\n", index);

    if (Up_go == 0)
    {
        //! End of the travel all queue
        Cap = 0;
        msgsnd(id, msg, msg_size, 0);
        printf("Wow!\nEvery herdsman went through the bom!\n");
    }
}


//! Herdsman at the top !//
void Herdsman_up(int id, int index, struct Message_t* msg)
{
    int msg_size = sizeof(*msg);
    // Проверяем, не лежит ли шапка
    //! Checking for hat existing
    if (Cap)
    {
        printf("Herdsman %d waiting for going up other herdsman!\n", index);
        msgrcv(id, msg, msg_size, HERDSMAN_IN_BOM, 0);
    }

    //! Первый в колонне блокирует вход снизу
    if (Down_go == 0)
    {
        printf("Herdsman number %d going down now! Be careful!\n");
        Down_go++;
        msgsnd(id, msg, msg_size, 0);
    }

    printf("I'm herdsman number %d going through the bom to the bottom!\n", index);
    Down_go--;

    // Последний в колонне разблокирует вход снизу ??????
    if (Down_go == 0)
    {
        //V_oper(sem_id, UP, 1);
        //V_oper(sem_id, LOCK_DOWN, 1);
    }
}



void Herdsman(int sem_id, int index, int num_of_herdmans)
{
    struct Message_t msg;
    printf("Hello, I'm herdsman number %d.\nAnd I'm going to the top of mountain!\n", index);
    Herdsman_down(sem_id, index, &msg);

    printf("Hello, I'm herdsman number %d.\nAnd I'm going to the top of mountain!\n", index);
    Herdsman_up(sem_id, index, &msg);

    printf("I'm herdsman number %d and I'm going away!\n", index);
}


int main(int argc, char** argv)
{
    int num_of_herdmans = 0;

    if (argc == 1)
        num_of_herdmans = 1;


    num_of_herdmans = strtol(argv[1], NULL, 10);

    //Creating new queue
    int id = msgget(IPC_PRIVATE, 0777);

    for (int i = 0; i < num_of_herdmans; ++i)
    {
        //! Creating runners
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