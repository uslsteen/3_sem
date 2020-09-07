#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main()
{
	pid_t check = getpid();
	pid_t check1 = getppid();

	printf("size of pid = %d\nsize of ppid = %d\n", sizeof(check), sizeof(check1));

	printf("pid:%d\nppid %d\n", check, check1);

	return 0;
}
