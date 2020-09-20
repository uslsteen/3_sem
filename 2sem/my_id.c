#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>


const int list_size = 8;
/*
void my_id(int argc, char* argv[])
{
	int i = 0;
	uid_t uid = 0;
	gid_t gid = 0;
	int process_check = 0;

	gid_t* list = (gid_t*)calloc(list_size, sizeof(list[0]));
	assert(list);

  //uid = getuid();
	//gid = getgid();

	if ((process_check = getgroups(list_size, list)) == -1)
	{
		printf("Error int func getgroups\n");
		abort();
	}

	printf("uid =  %d gid = %d\n", uid, gid);

	for (i = 0; i < list_size; ++i)
		printf("%d ", list[i]);
}
*/

void get_id(int argc, char* argv[])
{
	struct passwd* res = NULL;
	struct group* res_gr = NULL;

	int i = 1;
	int process_check = 0;

	gid_t* list = (gid_t*)calloc(list_size, sizeof(list[0]));
	assert(list);

	res = getpwnam(argv[i]);

	if ((process_check = getgroups(list_size, list)) == -1)
	{
		printf("Error int func getgroups\n");
		abort();
	}

	printf("uid = %d(%s), gid = %d(%s) ", res->pw_uid, res->pw_name, res->pw_gid, res->pw_name);

	printf("группы = ");

	res_gr = getgrgid(list[list_size - 1]);
	printf("%d(%s), ", list[list_size - 1], res_gr->gr_name);

	for (i = 0; i < list_size - 1; ++i)
	{
		res_gr = getgrgid(list[i]);

		printf("%d(%s) ", list[i], res_gr->gr_name);

		if (i != list_size - 2)
			printf(", ");
	}

	printf("\n");
}


int main(int argc, char* argv[])
{
	//my_id(argc, argv);
	get_id(argc, argv);

	return 0;
}
