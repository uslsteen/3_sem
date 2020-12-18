# 3sem
Task from working in 3sem.

## first seminar
```my_echo.c``` - its clone of linux cmd echo.

```hello.c``` - standart first program on C with help func:

```bash
ssize_t write(int fildes, const void *buf, size_t nbytes)
```


## second seminar
```my_id.c``` - my realisation linux cmd id

## third seminar

```my_cat.c``` - my realisation linux cmd cat with help funcs:

```bash
int open(const char *pathname, int flags, mode_t mode);
```

```bash
ssize_t write(int fd, const void *buf, size_t count);  
```

```bash
ssize_t read(int fd, void *buf, size_t count);
 
```
```my_cp.c``` - it should be realistion linux cmd cp with help same funcs.

## fourth seminar

```my_fork.c``` - creating process with help function:

```bash
pid_t fork(void)
```
fork() creates a new process by duplicating the calling process.  The new process is referred to as the child process.  The calling process is referred to as the parent process.

```my_exec.c``` - execution of linux terminal comand as echo, cat etc.
This program work with help function:

```bash
int execv(const char *path, char *const argv[]);
```

## fifth seminar

```my_pipe.c``` - realisation implementation of a unidirectional channel, which is required for interprocess communication.

## sixth seminar

```runners_proc.c``` - program of simulating runners competition. 

There is ```num_of_runners``` , who are created with help syscall ```fork()```.

The judge gives commands to the runners.

Runners, in the order of their arrival at the start, perform a sequence of actions: start, end the race.
In this programm I used new syscalls:

```bash
int msgsnd(int msqid, struct msgbuf *msgp, size_t msgsz, int msgflg);
```

```bash
ssize_t msgrcv(int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflg);
```

```bash
int msgget(key_t key, int msgflg); 
```


## seventh seminar

```my_boat.c``` - programm - realisation of trip on the boat. 

Captain gives commans to the passenger about arrival and departure, about condition of the stair. 

Passengers make step to stair (take sem STAIR), then step to boat (put STAIR, take sem BOAT) and etc.

[in] parametrs - num_of_passengers, capacity of boat, capacity of stair, num of trips.

In this program I used new syscalls:

```bash
int semget(key_t key, int nsems, int semflg);  
```

```bash
ssize_t msgrcv(int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflg);
```

And also treee atomic operations written by myself (see in ```semaphors.h```).



## eighth seminar

```my_client.c``` and ```my_server.c``` - program - simulation of working real server with calls from client.

You should run ```client.c``` in one terminal, ```server.c``` in another termianl and see amazing relationship between it.

It this program I knew about shared memory concept and used this new syscalls:

```bash
key_t ftok(const char *pathname, int proj_id);  
```

```bash
int shmget(key_t key, int size, int shmflg);
```

```bash
void *shmat(int shmid, const void *shmaddr, int shmflg);
```

And also treee atomic operations written by myself (see in ```semaphors.h```).

## Program from contorl work (bridge_simulation)

```bridge_simulation.c``` - its a program-simalution of collision resolution between car and ship on the bridge.

In this program I used all my knoledge about semaphors and shared memory from last seminars.

