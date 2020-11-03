//! Heroes of this program

void Captain(int sem_id, int num_of_trips, int cap_of_stair, int cap_of_boat)
{
    //! Init of Semaphors
    V_oper(sem_id, STAIR_DOWN, 1);
    V_oper(sem_id, START_OF_TRIP, 1);
    V_oper(sem_id, END_OF_TRIP, 1);
    V_oper(sem_id, NUM_ON_STAIR, cap_of_stair);
    //! End of initialisation sems

    //! Boat preparing
    printf("Hello, I'm your captain!\n");
    printf("Trap is going down!\n");

    P_oper(sem_id, STAIR_DOWN, 1);

    for (int i = 0; i < num_of_trips; ++i)
    {
        if (i == num_of_trips - 1)
            V_oper(sem_id, LAST_TRIP, 1);

        printf("Welcome to the boat!\n");

        Z_oper(sem_id, NUM_ON_STAIR);
        //! Captain is waiting for people
        Z_oper(sem_id, NUM_ON_BOAT);

        printf("Captain is waiting for the stair to be removed\n");
        printf("Stair is going up!\n");
        V_oper(sem_id, STAIR_DOWN, 1);

        P_oper(sem_id, START_OF_TRIP, 1);
        printf("Let's start our trip!\n");

        printf("        *** Two hours later ***        \n");
        P_oper(sem_id, END_OF_TRIP, 1);

        printf("Ladies ang gentelments, our trip is over!\nGo away from my boat!\n");

        //! Captain down the stair
        P_oper(sem_id, STAIR_DOWN, 1);

        Z_oper(sem_id, NUM_ON_BOAT);

        Z_oper(sem_id, NUM_ON_STAIR);

        Z_oper(sem_id, NUM_ON_COAST);

        //! Captain is preparing boat for new quant of pepople
        printf("Wow, the boat is finally empty. I have finished another journey.\n");

        V_oper(sem_id, START_OF_TRIP, 1);
        V_oper(sem_id, END_OF_TRIP, 1);

        P_oper(sem_id, NUM_ON_COAST, cap_of_boat);


        if (i == num_of_trips - 1)
            break;
    }

    printf("Captain go away!\n");
}

void Passengers(int sem_id, int index)
{
        printf("Hello, I'm passenger number %d\n", index);
        //! Waiting for stair(NULL)
        Z_oper(sem_id, STAIR_DOWN);


        P_oper(sem_id, NUM_ON_STAIR, 1);
        printf("Passenger number %d stepped in the stair!\n", index);

        P_oper(sem_id, NUM_ON_BOAT, 1);
        printf("Passenger nuber %d stepped into the boat!\n", index);

        //! Passenger[index] waiting for the begging of his trip
        Z_oper(sem_id, START_OF_TRIP);

        //! Passenger[index] waiting for the ending of his trip
        Z_oper(sem_id, END_OF_TRIP);

        //! Passenger[index] waiting for the down of stair
        Z_oper(sem_id, STAIR_DOWN);

        //! Passenger[index] go away from boat
        V_oper(sem_id, NUM_ON_BOAT, 1);
        printf("Passenger number %d have gone on the stair\n", index);

        //! Passenger[index] go away from stair
        V_oper(sem_id, NUM_ON_STAIR, 1);
        printf("Passenger number %d have gone on the island\n", index);

        P_oper(sem_id, NUM_ON_COAST, 1);


        printf("Passenger number %d is glad for trip on the boat!\n", index);
}



int main(int argc, char** argv)
{
    int num_of_pass = 5,
            num_of_trips = 1,
            cap_of_boat = 5,
            cap_of_stair = 5;

  if (argc >= 5)
  {
  //! Getting arguments of terminal
    num_of_pass = strtol(argv[1], NULL, 10),
    num_of_trips = strtol(argv[2], NULL, 10),
    cap_of_boat = strtol(argv[3], NULL, 10),
    cap_of_stair = strtol(argv[4], NULL, 10);
  }


    int sem_id = semget(IPC_PRIVATE, num_of_semaphors, 0777);
    if (sem_id < 0)
    {
        perror("Semget return error!\n");
        return ERROR;
    }

    //Num_processing(cap_of_boat, num_of_pass, sem_id);

    if (cap_of_boat <= num_of_pass)
    {
        V_oper(sem_id, NUM_ON_COAST, cap_of_boat);
        V_oper(sem_id, NUM_ON_BOAT, cap_of_boat);
    }
    else
    {
        V_oper(sem_id, NUM_ON_COAST, num_of_pass);
        V_oper(sem_id, NUM_ON_BOAT, num_of_pass);
    }

    pid_t captains_pid = fork();

    if (captains_pid < 0)
    {
        perror("Captains pid isnt zero!\n");
        return ERROR;
    }

    if (captains_pid == 0)
    {
        if (cap_of_boat <= num_of_pass)
            Captain(sem_id, num_of_trips, cap_of_stair, cap_of_boat);

        else if (cap_of_boat > num_of_pass)
            Captain(sem_id, num_of_trips, cap_of_stair, num_of_pass);

        return 0;
    }

    for (int i = 0; i < num_of_pass; ++i)
    {
        pid_t passngrs_pid = fork();

        if (passngrs_pid < 0)
        {
            perror("Passengers pid not zero!\n");
            return ERROR;
        }

        else if (passngrs_pid == 0)
        {
            Passengers(sem_id, i + 1);
            return 0;
        }

    }
    for (int i = 0; i < num_of_pass + 1; ++i)
    {
        if (wait(NULL) < 0)
        {
            perror("wait error!\n");
            return ERROR;
        }
    }

    semctl(sem_id, 0, IPC_RMID);
    return 0;

}
