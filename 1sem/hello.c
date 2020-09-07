#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define ST_OUT 1

int main()
{

	//first version of "Hello,world!"
	/*
	printf("Hello, world!\n");
	return 0;
	*/

	//second version of "Hello, world!"
	char* test = "Hello, world!\n";

	write(ST_OUT, test, strlen(test));

	return 0;
}
