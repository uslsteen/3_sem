#include "sems.h"


//! Herdsman in the bottom !//
void Herdsman_down(int id, int index, struct Message_t* msg)
{
    int msg_size = sizeof(*msg);
    //msg->value = HERDSMAN_CAME;
    // msgsnd(id &msg msg_size, 0);
    //P_oper(sem_id, LOCK_UP, 1);
    Upwait++;

    /*
    if (Upwait != 1)
    {
        msg->value = index;
        msgsnd(id, &msg, msg_size, 0);
        //! Herdsman not first in queue

        V_oper(sem_id, LOCK_UP, 1);
        P_oper(sem_id, UP_QUEUE, 1);
        P_oper(sem_id, LOCK_UP, 1);

    }*/

    if (!Cap)
    {
        //! Оставляем стадо и идем вверх;
        //! Кладем шапку;
        printf("Herdsman number %d left your stado and went to the of mountain!\n", index);

        //P_oper(sem_id, LOCK_UP, 1);
        Cap = 1;
        printf("Herdsman number %d put your hat on the ground!\n", index);
        printf("Herdsman number %d went to the down of mountain!\n", index);

        //! Идем обратно;
        //! ждем возможности входа
        if (Down_go != 0)
        {
            printf("Herdsman is waiting for free of bom!\n");
            msgrcv(id, &msg, msg_size, index, 0);
        }
        //! Herdsman prepare for travel through the bom
    }

    //! формирование колонны
    //! Resizing of herdsmans queue
    Upwait--;

    /*
    if (Upwait != 0)
    {
        //V_oper(sem_id, UP_QUEUE, 1);
        Up_go++;
        //V_oper(sem_id, LOCK_UP, 1);
    }
    */

    printf("I'm herdsman number %d and I'm going through the bom to the top!\n", index);
    Cap = 0;
    msgsnd(id, &msg, msg_size, 0);
    Up_go--;

    if (Up_go == 0)
    {
        // конец прохода колонны
        Cap = 0;
        //V_oper(sem_id, DOWN, 1);
        //! msgsnd(id, &msg, msg_size, 0)
        printf("Wow!\nEvery herdsman went through the bom!\n");
    }

    //V_oper(sem_id, LOCK_UP, 1);
}


//! Herdsman at the top !//
void Herdsman_up(int id, int index, struct Message_t* msg)
{
    int msg_size = sizeof(*msg);
    // Проверяем, не лежит ли шапка
    if (Cap)
    {
        msgrcv(id, &msg, msg_size, index, 0);
        printf("Herdsman %d waiting for going up otрer herdsman!\n");
    }
    //P_oper(sem_id, DOWN, 1);
    //P_oper(sem_id, LOCK_DOWN, 1);

    // Первый в колонне блокирует вход снизу

    if (Down_go == 0)
    {
        printf("Herdsman number %d going down now! Be careful!\n");
        Down_go++;
        msgsnd(id, &msg, msg_size, 0);
    }

    printf("I'm herdsman number %d and I'm going through the bom to the bottom!\n", index);

    //P_oper(sem_id, LOCK_DOWN, 1);
    Down_go--;

    // Последний в колонне разблокирует вход снизу ??????
    if (Down_go == 0)
    {
        V_oper(sem_id, UP, 1);
        V_oper(sem_id, LOCK_DOWN, 1);
    }
}

/*
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
 */

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


/*
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
 */
