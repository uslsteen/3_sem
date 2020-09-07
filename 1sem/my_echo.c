#include <stdio.h>
#include <string.h>
#include <stdlib.h>


enum COND
{
	UP = 0,
	DOWN,
};

void echo_call(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	echo_call(argc, argv);

	return 0;
}


void echo_call(int argc, char* argv[])
{
	if (argc > 1)
	{
		int i = 1, flag = DOWN;

		if (strcmp(argv[i], "-n") == 0)
		{
			i++;
			flag = UP;
		}


		for (; i < argc; ++i)
		{
			printf("%s", argv[i]);

			if (i != argc - 1)
			{
				printf(" ");
			}
		}

		if (flag == DOWN)
			printf("\n");
	}
        else
		printf("\n");
}
