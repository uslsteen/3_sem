#include "semaphs.h"


void Car_method(int sem_id, int index,  struct Bridge_organization* shm_struct)
{
    P_oper(sem_id, SHRD_VAR, 1);

    (shm_struct->num_of_waiting_cars)++;
		printf("At the moment driver[%d] stay in queue!\n\n", index);

    V_oper(sem_id, SHRD_VAR, 1);

    P_oper(sem_id, CAR, 1);
    P_oper(sem_id, BRIDGE, 1);

		printf("Driver[%d] gonna drive through the bridge!\n\n", index);

    P_oper(sem_id, SHRD_VAR, 1);

    (shm_struct->num_of_waiting_cars)--;
		printf("Driver[%d] drove the bridge!\n\n", index);

    if (shm_struct->num_of_waiting_ships > MAX_SHIPS || ((shm_struct->num_of_waiting_cars == 0) && (shm_struct->num_of_waiting_ships != 0)))
    {
        V_oper(sem_id, SHIP, 1);

        //! Checking for condition of the bride: close or open?
        if (shm_struct->bridge_cond == CLOSE) //
        {
            printf("The employees of the transport system in a hurry raise the bridge!\n\n");
            shm_struct->bridge_cond = OPEN;

            printf("Success, employees raised the bridge!\n\n");
        }
    }
    else
    {
        V_oper(sem_id, CAR, 1);

        if (shm_struct->bridge_cond == OPEN)
        {
            printf("Employees of the transport system in a hurry lower the bridge.\n");
            shm_struct->bridge_cond = CLOSE;

            printf("Success, employees lowed the bridge!\n\n");
        }
    }

    V_oper(sem_id, SHRD_VAR, 1);
    V_oper(sem_id, BRIDGE, 1);

    return;
}

void Ship_method(int sem_id, int index, struct Bridge_organization* shm_struct)
{
    P_oper(sem_id, SHRD_VAR, 1);

    (shm_struct->num_of_waiting_ships)++;
		printf("At the moment captain of the ship[%d] stay in queue!\n\n", index);

    V_oper(sem_id, SHRD_VAR, 1);


    P_oper(sem_id, SHIP, 1);
    P_oper(sem_id, BRIDGE, 1);

		printf("Captain of the ship[%d] gonna swim under the bridge!\n\n", index);

    P_oper(sem_id, SHRD_VAR, 1);

    (shm_struct->num_of_waiting_ships)--;
		printf("Captain of the ship[%d] swome under the bridge!\n\n", index);

    if (shm_struct->num_of_waiting_ships > MAX_SHIPS || ((shm_struct->num_of_waiting_cars == 0) && (shm_struct->num_of_waiting_ships != 0)))
    {
        V_oper(sem_id, SHIP, 1);

        if ((shm_struct->bridge_cond) == CLOSE)
        {
            printf("The employees of the transport system in a hurry raise the bridge!\n\n");
            shm_struct->bridge_cond = OPEN;

            printf("Success, employees raised the bridge!\n\n");
        }

    }
    else
    {
        V_oper(sem_id, CAR, 1);

        if (shm_struct->bridge_cond == OPEN)
        {
            printf("Employees of the transport system in a hurry lower the bridge.\n");
            shm_struct->bridge_cond = CLOSE;

            printf("Success, employees lowed the bridge!\n\n");
        }

    }

    V_oper(sem_id, SHRD_VAR, 1);
    V_oper(sem_id, BRIDGE, 1);

    return;
}

void Init(struct Bridge_organization* shm_struct, int sem_id)
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
    int num_of_cars = 5, num_of_ships = 6, sem_id = 0, shm_id = 0;
		pid_t pid = 0;

    if (argc == 3)
    {
        num_of_cars = atoi(argv[1]);
        num_of_ships = atoi(argv[2]);
    }

    char print_buf[BUFFER_SIZE];
    setvbuf(stdout, print_buf, _IOLBF, BUFFER_SIZE);


		if ((sem_id = semget(IPC_PRIVATE, num_of_semaphors, MAX_PERMISSION)) < 0)
			return Err_proc("semget return negative value!\n");

		if ((shm_id = shmget(IPC_PRIVATE, BUFSIZ, MAX_PERMISSION | IPC_PRIVATE)) < 0)
			return Err_proc("shmget return negative value!\n");

		struct Bridge_organization* shm_struct = (struct Bridge_organization*) calloc(1, sizeof(struct Bridge_organization));
		assert(shm_struct);

		shm_struct = (struct Bridge_organization*) shmat(shm_id, NULL, 0);

    if ((void*) shm_struct == (void*) -1)
    	return Err_proc("shmat return neg value!\n");

		Init(shm_struct, sem_id);

    printf("Start simulating!\n");

    for (int index = 0; index < num_of_cars + num_of_ships; index++)
    {
        if ((pid = fork()) < 0)
					return Err_proc("fork return neg value to pid!\n");

        if (!pid)
        {
            if (index < num_of_cars)
                Car_method(sem_id, index + 1, shm_struct);

            else Ship_method(sem_id, index - num_of_cars + 1, shm_struct);

            return 0;
        }
    }


    for (int index = 1; index < num_of_cars + num_of_ships; index++)
		{
        if (wait(NULL) < 0)
					return Err_proc("wait return negative value!\n");
		}


		//! Remove shared memory and semaphors
		if (semctl(sem_id, num_of_semaphors, IPC_RMID) < 0)
        return Err_proc("semctl return negative value!\n");


    if (shmctl(shm_id, IPC_RMID, (struct shmid_ds *)shm_struct ) < 0)
        return Err_proc("shmctl return negative value!\n");


    printf("End of simualting!\n");

    return 0;
}
