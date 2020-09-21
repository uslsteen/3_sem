#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>


const int MAX_LIST_SIZE = 50;
const int ERROR = -1;
const int IS_DIGIT = 0;
const int ISNT_DIGIT = 1;

//! struct for union struct passwd and struct group
typedef struct User_info
{
    struct passwd* psswd;
    struct group* grp;

    gid_t* gr_list;
    int gr_list_size;

} User_info;


//! Function for call my_id without args
//! @param[in] user - pointer to structure of user_info
//! @return true if func executed rigth
bool get_id_wihout_args(User_info* user)
{
    user->psswd = getpwuid(getuid());
    user->grp = getgrgid(getgid());

    if (user->psswd == NULL || user->psswd == NULL)
    {
        printf("Error in getpwuid(getuid()) or getgrgid(getgid()).\n");
        printf("Check this block of code\n");

        return false;
    }

    if ((user->gr_list_size = getgroups(MAX_LIST_SIZE, user->gr_list)) == -1)
    {
        printf("Error in func getgroups.\n");
        return false;
    }

    return true;
}

//! Function for call my_id with args
//! @param[in] user - pointer to structure of user_info
//! @param[in] argv - pointer to buffer argv
//! @return true if func executed rigth
bool get_id_with_args(char* argv, User_info* user)
{

    if (isdigit(argv[0]))
    {
      uid_t uid = strtol(argv, NULL, 0);
      user->psswd = getpwuid(uid);
    }
    else
      user->psswd = getpwnam(argv);


    if (user->psswd == NULL)
    {
        printf("id: %s : no such user\n", argv);
        return false;
    }

    user->grp = getgrgid(user->psswd->pw_gid);

    if (user->grp == NULL)
    {
        printf("Func getgrgid() return NULL.\n");
        abort();
    }

    if ((user->gr_list_size = getgrouplist(user->psswd->pw_name, user->psswd->pw_gid, user->gr_list, &(user->gr_list_size))) == -1)
    {
        printf("Error in func getgroupd.\n");
        return false;
    }

    return true;
}

//! Constructor for user_info structure
//! @param[in] user - pointer to structure of user_info
void User_constructor(User_info* user)
{
    user->psswd = NULL;
    user->grp = NULL;

    user->gr_list_size = MAX_LIST_SIZE;

    user->gr_list = (uid_t*)calloc(user->gr_list_size, sizeof(uid_t));
    assert(user->gr_list);
}


//! Function for print user info
//! @param[in] user - pointer to structure of user_info
void print_usr_info(User_info* user)
{
    printf("uid = %d(%s) ", user->psswd->pw_uid, user->psswd->pw_name);
    printf("gid = %d(%s), ", user->grp->gr_gid, user->grp->gr_name);
    int list_size = user->gr_list_size;

    printf("группы = ");

    struct group* res_gr = getgrgid(user->gr_list[list_size - 1]);
    printf("%d(%s), ", user->gr_list[list_size - 1], res_gr->gr_name);

    for (int i = 0; i < list_size - 1; ++i)
    {
        res_gr = getgrgid(user->gr_list[i]);

        printf("%d(%s)", user->gr_list[i], res_gr->gr_name);

        if (i != list_size - 2)
            printf(", ");
    }

    printf("\n");
}


//! Distructor for user_info structure
//! @param[in] user - pointer to structure of user_info
void User_distructor(User_info* user)
{
    free(user->gr_list);

    user->psswd = NULL;
    user->grp = NULL;

    user->gr_list_size = 0;
}

int main(int argc, char** argv)
{
    User_info user;

    User_constructor(&user);

    if (argc == 1)
    {
        if(!get_id_wihout_args(&user))
        {
            printf("Get_id_without_args returned false.\nSomething went frong\n");
            return ERROR;
        }
    }
    else if (argc == 2)
    {
        if (!get_id_with_args(argv[1], &user))
            return ERROR;
    }
    else
        printf("id: extra operand \"\"");

    print_usr_info(&user);

    User_distructor(&user);

    return 0;
}
