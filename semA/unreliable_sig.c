#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

int reader_pid, writer_pid;

int fd;
int f_n;

char **f_names;


int Err_proc(char* err_string)
{
    perror(err_string);
    return errno;
}

void Reader_method()
{
	static int c = 0;
	static char i = 0;

  if (!(i &= 0x7))
  {
    int size = 0;

		while (!(size = read(fd, &c, 1)))
    {
      close(fd);

			if (!f_n--)
      {
				kill(writer_pid, SIGTERM);
				return;
			}

			if ((fd = open(*(f_names++), S_IRUSR)) == -1)
      {
        Err_proc("open return negative value!\n");
        return;
      }
    }

    if (size < 0)
    {
      Err_proc("Read error:\n");
      return;
    }


	}

	++i;
	kill(writer_pid, (c & 1) ? SIGUSR2 : SIGUSR1);
	c >>= 1;
}


void Writer_method(int signal)
{
	static char var = 0;
	static char index = 0;

	var += ((signal == SIGUSR2) ? 1 : 0) << index;
	++index;

	if (!(index &= 0x7))
  {
    putchar(var);
	  var = 0;
	}

	kill(reader_pid, SIGUSR1);
}


int main(int argc, char **argv)
{
	setbuf(stdout, NULL);

	reader_pid = getpid();
	writer_pid = fork();

  if (writer_pid < 0)
    Err_proc("fork() return negative value!\n");


	if (writer_pid == 0)
  {
		signal(SIGUSR1, Writer_method);
		signal(SIGUSR2, Writer_method);

		kill(reader_pid, SIGUSR1);

		while (pause());
	}

	if (argc > 1)
  {
		if ((fd = open(argv[1], S_IRUSR)) == -1)
      return Err_proc("open returned negative value!\n");

		f_names = &argv[2];

		f_n = argc - 2;
	}

	else fd = STDIN_FILENO;

	signal(SIGUSR1, Reader_method);
	wait(NULL);
}
