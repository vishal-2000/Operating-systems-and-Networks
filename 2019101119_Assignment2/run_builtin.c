#include "changedir.h"
#include "echo_com.h"
#include "ls_com.h"
#include "pinfo_com.h"
#include "headers.h"

void run_builtin(char* subtoken, char* temp2)
{
    //char* temp2;
    //char* subtoken = strtok_r(token, " ", &temp2);
    //printf("Subtoken: %s\n", subtoken);
    if(strcmp(subtoken, "cd")==0) // cd command
    {
            subtoken = strtok_r(temp2, " ", &temp2);
            changedir(subtoken);
    }
    else if(strcmp(subtoken, "pwd")==0) // pwd command
    {
        char presentdir[PATH_MAX];
        getcwd(presentdir, PATH_MAX);
        printf("%s\n", presentdir);
    }
    else if(strcmp(subtoken, "echo")==0) // echo command
    {
        echo_com(temp2);
    }
    else if(strcmp(subtoken, "ls")==0)
    {
        ls_com(temp2);
    }
    else if(strcmp(subtoken, "pinfo")==0)
    {
        pinfo_com(temp2);
    }
}