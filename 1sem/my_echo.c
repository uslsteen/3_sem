#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void echo_call(int argc, char* argv[])
{
	if (argc > 1)
	{
		int i = 1;
		bool flag = true;

		if (strcmp(argv[i], "-n") == 0)
		{
			i++;
			flag = false;
		}


		for (; i < argc; ++i)
		{
			printf("%s", argv[i]);

			if (i != argc - 1)
				printf(" ");
		}

		if (flag)
			printf("\n");
	}
        else
		printf("\n");
}


int main(int argc, char* argv[])
{
	echo_call(argc, argv);

	return 0;
}
