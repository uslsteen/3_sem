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

