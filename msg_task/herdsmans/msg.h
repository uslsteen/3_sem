//
// Created by anon on 29.11.2020.
//
#ifndef VORUSH_CODE_SEMS_H
#define VORUSH_CODE_SEMS_H

#define HRDSMAN_DWN_LET_GO (num_of_herdmans + 4)

#define HRDSMAN_DWN_WTNG (num_of_herdmans + 3)

#define HRDSMAN_IN_BOM_UP (num_of_herdmans + 2)

#define HRDSMAN_IN_QUEUE (num_of_herdmans + 1)


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>


#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/msg.h>


const int ERROR = -1;


int Up_wait = 0;  //! Variable for num of waiting herdsmans
int Up_go = 0;    //! Var for num of going up herdsmans
int Down_go = 0;  //! Var for num of going down herdsmans
int Cap = 0;         //! Flag for hat on the top enter


struct Message_t
{
    long value; /* тип сообщения, должен быть > 0 */
};

//! Herdsman in the bottom !//
void Herdsman_at_the_bot(int id, int index, struct Message_t* msg, int num_of_herdmans)
{
    int msg_size = sizeof(*msg);
    Up_wait++;

    //! Herdsman[index] not first in queue
    //! Therefore herdsman[index] is waiting for the others
    if (Up_wait != 1)
    {
        printf("Herdsman number %d waiting in queue others herdsmans!\n", index);
        msgrcv(id, msg, msg_size, HRDSMAN_IN_QUEUE, 0);
    }

    if (!Cap)
    {
        //! Heardsman[index] going to the top of mountain
        printf("Herdsman number %d left his sheeps and went to the top of mountain!\n", index);
        Cap = 1;

        printf("Herdsman number %d put your hat on the ground!\n", index);
        printf("Herdsman number %d went to the bot of mountain!\n", index);

        //! Heardsman[index] going back and waiting for possibility enter to the bom
        if (Down_go != 0)
        {
            printf("Herdsman is waiting for free of bom!\n");
            msgrcv(id, msg, HRDSMAN_DWN_WTNG, index, 0);
            msgrcv(id, msg, HRDSMAN_DWN_LET_GO, index, 0);
        }
    }

    //! Herdsman[index] prepare for travel through the bom
    //! Resizing of herdsmans queue
    Up_wait--;

    //! Processing for queue
    //! Here herdsman[index] send message to other heardsmans about decreasing queue
    if (Up_wait != 0)
    {
        msg->value = HRDSMAN_IN_QUEUE;
        msgsnd(id, msg, msg_size, 0);
    }

    Up_go++;
    //! Herdsman[index] took your hat ang go to the land

    //! Herdsman[index] send message to his colleague in the top of mountain
    msg->value = HRDSMAN_IN_BOM_UP;
    msgsnd(id, msg, msg_size, 0);
    Cap = 0;

    Up_go--;
    printf("I'm herdsman number %d going through the bom to the top!\n", index);

    if (Up_go == 0)
    {
        //! End of the travel all queue
        Cap = 0;
        msgsnd(id, msg, msg_size, 0);
        //printf("Wow!\nEvery herdsman went through the bom!\n");
    }
}


//! Herdsman at the top !//
void Herdsman_at_the_top(int id, int index, struct Message_t* msg, int num_of_herdmans)
{
    int msg_size = sizeof(*msg);

    //! Checking for hat existing
    if (Cap)
    {
        printf("Herdsman %d waiting for going up other herdsman!\n", index);
        msgrcv(id, msg, msg_size, HRDSMAN_IN_BOM_UP, 0);
    }

    //! Первый в колонне блокирует вход снизу
    //! FIRST Herdsman[index] of their company info other herdsmans in the bottom of mountain about beggining of his travel
    if (Down_go == 0)
    {
        msg->value = HRDSMAN_DWN_WTNG;
        msgsnd(id, msg, HRDSMAN_DWN_WTNG, 0);
        printf("Herdsman number %d going down now! Be careful!\n", index);
        Down_go++;
    }

    printf("I'm herdsman number %d going through the bom to the bottom!\n", index);
    Down_go--;

    //! LAST Herdsman[index] of their company info other herdsmans in the bottom of mountain about ending of his travel
    if (Down_go == 0)
    {
        msg->value = HRDSMAN_DWN_LET_GO;
        msgsnd(id, msg, HRDSMAN_DWN_LET_GO, 0);
    }
}




#endif //VORUSH_CODE_SEMS_H
