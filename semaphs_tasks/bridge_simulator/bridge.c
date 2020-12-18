#include "semaphs.h"

//! Function of simulating car existing
//! \param sem_id
//! \param index
//! \param shm_struct
//! void function - no return
void Car_method(int sem_id, int index,  struct Bridge_orgnz* shm_struct)
{
    //! Driver[index] moved to begin of cars queue

    //! Driver[index] take sem SHRD_VAR for working with shm_struct
    P_oper(sem_id, SHRD_VAR, 1);

    (shm_struct->num_of_waiting_cars)++;
    printf("At the moment driver[%d] stay in queue!\n\n", index);
    sleep(DELAY);

    V_oper(sem_id, SHRD_VAR, 1);
    //! Driver[index] end working with shm_struct

    P_oper(sem_id, CAR, 1);

    //! Driver[index] take sem for riding over the bridge
    P_oper(sem_id, BRIDGE, 1);

    printf("Driver[%d] gonna drive over the bridge!\n\n", index);
    sleep(DELAY);

    //!
    P_oper(sem_id, SHRD_VAR, 1);

    (shm_struct->num_of_waiting_cars)--;
    printf("Driver[%d] drove over the bridge!\n\n", index);
    sleep(DELAY);

    if (shm_struct->num_of_waiting_ships > MAX_NUM_OF_SHIPS || ((shm_struct->num_of_waiting_cars == 0) && (shm_struct->num_of_waiting_ships != 0)))
    {
        V_oper(sem_id, SHIP, 1);

        //! Checking for condition of the bride: close or open?
        if (shm_struct->bridge_cond == CLOSE) //
        {
            printf("The employees of the transport system in a hurry raise the bridge!\n\n");
            sleep(DELAY);
            shm_struct->bridge_cond = OPEN;

            printf("Success, employees raised the bridge!\n\n");
            sleep(DELAY);
        }
    }
    else
    {
        V_oper(sem_id, CAR, 1);

        //! Checking for condition of the bride: close or open?
        if (shm_struct->bridge_cond == OPEN)
        {
            printf("Employees of the transport system in a hurry lower the bridge.\n");
            sleep(DELAY);
            shm_struct->bridge_cond = CLOSE;

            printf("Success, employees lowed the bridge!\n\n");
            sleep(DELAY);
        }
    }


    V_oper(sem_id, SHRD_VAR, 1);
    V_oper(sem_id, BRIDGE, 1);
    //! Driver[index] ended working with shm struct and leave the bridge

    return;
}

//! Function of simulating ship existing
//! \param sem_id
//! \param index
//! \param shm_struct
void Ship_method(int sem_id, int index, struct Bridge_orgnz* shm_struct)
{
    P_oper(sem_id, SHRD_VAR, 1);

    (shm_struct->num_of_waiting_ships)++;
    printf("At the moment captain of the ship[%d] stay in queue!\n\n", index);
    sleep(DELAY);

    V_oper(sem_id, SHRD_VAR, 1);


    P_oper(sem_id, SHIP, 1);
    P_oper(sem_id, BRIDGE, 1);

    printf("Captain of the ship[%d] gonna swim under the bridge!\n\n", index);
    sleep(DELAY);

    P_oper(sem_id, SHRD_VAR, 1);

    (shm_struct->num_of_waiting_ships)--;
    printf("Captain of the ship[%d] swome under the bridge!\n\n", index);
    sleep(DELAY);

    if (shm_struct->num_of_waiting_ships > MAX_NUM_OF_SHIPS || ((shm_struct->num_of_waiting_cars == 0) && (shm_struct->num_of_waiting_ships != 0)))
    {
        V_oper(sem_id, SHIP, 1);

        if ((shm_struct->bridge_cond) == CLOSE)
        {
            printf("The employees of the transport system in a hurry raise the bridge!\n\n");
            sleep(DELAY);
            shm_struct->bridge_cond = OPEN;

            printf("Success, employees raised the bridge!\n\n");
            sleep(DELAY);
        }

    }
    else
    {
        V_oper(sem_id, CAR, 1);

        if (shm_struct->bridge_cond == OPEN)
        {
            printf("Employees of the transport system in a hurry lower the bridge.\n");
            sleep(DELAY);
            shm_struct->bridge_cond = CLOSE;

            printf("Success, employees lowed the bridge!\n\n");
            sleep(DELAY);
        }

    }

    V_oper(sem_id, SHRD_VAR, 1);
    V_oper(sem_id, BRIDGE, 1);
    //! Captain of the ship[index] ended working with shm struct and leave place with bridge

    return;
}

//! Function of initialization semaphors and shm struct
//! \param shm_struct
//! \param sem_id
void Init(struct Bridge_orgnz* shm_struct, int sem_id)
{
    shm_struct->num_of_waiting_cars = 0;
    shm_struct->num_of_waiting_ships = 0;
    shm_struct->bridge_cond = CLOSE;

    V_oper(sem_id, SHRD_VAR, 1);
    V_oper(sem_id, BRIDGE, 1);
    V_oper(sem_id, CAR, 1);
}



int main(int argc, char** argv)
{
    int num_of_cars = 5, num_of_ships = 6, sem_id = 0, shm_id = 0, status = 0;
    pid_t pid = 0;

    //! Getting arguments from the console
    if (argc == 3)
    {
        num_of_cars = atoi(argv[1]);
        num_of_ships = atoi(argv[2]);
    }
    char print_buf[BUFFER_SIZE];
    setvbuf(stdout, print_buf, _IOLBF, BUFFER_SIZE);

    //! Creating semaphore set identifier associated with IPC_PRIVATE
    if ((sem_id = semget(IPC_PRIVATE, NUM_OF_SEMAPHORS, MAX_PERMISSION)) < 0)
        return Err_proc("semget return negative value!\n");

    //! Creating shared memory segment associated with IPC_PRIVATE
    if ((shm_id = shmget(IPC_PRIVATE, BUFSIZ, MAX_PERMISSION | IPC_PRIVATE)) < 0)
        return Err_proc("shmget return negative value!\n");

    struct Bridge_orgnz* shm_struct = (struct Bridge_orgnz*) calloc(1, sizeof(struct Bridge_orgnz));
    assert(shm_struct);

    shm_struct = (struct Bridge_orgnz*) shmat(shm_id, NULL, 0);

    //! Checking for error from shmat
    if ((void*) shm_struct == (void*) -1)
        return Err_proc("shmat return neg value!\n");

    //! Initialization semaphors and shm_struct
    Init(shm_struct, sem_id);

    printf("Start simulating!\n");

    //! Creating process and calls car and ship methods
    for (int index = 0; index < num_of_cars + num_of_ships; index++)
    {
        if ((pid = fork()) < 0)
            return Err_proc("fork return neg value to pid!\n");

        if (pid == 0)
        {
            if (index % 2)
                Car_method(sem_id, (index / 2) + 1, shm_struct);

            else Ship_method(sem_id, (index + 1)/2 + 1, shm_struct);

            return 0;
        }
    }

    for (int index = 0  ; index < num_of_cars + num_of_ships; index++)
        wait(&status);

    //! Remove shared memory and semaphors
    if (semctl(sem_id, NUM_OF_SEMAPHORS, IPC_RMID) < 0)
        return Err_proc("semctl return negative value!\n");


    if (shmctl(shm_id, IPC_RMID, (struct shmid_ds *)shm_struct ) < 0)
        return Err_proc("shmctl return negative value!\n");


    printf("End of simualting!\n");

    return 0;
}
