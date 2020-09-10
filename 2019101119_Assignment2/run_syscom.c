#include "headers.h"
#include <sys/types.h> 
#include <sys/wait.h>
#include "ls_com.h"

int run_syscom(char* str1, char* str2) // returns pid of background process returns 0(in case of fore ground process)
{

    char ** args  = NULL;
    args = (char**)realloc(args, sizeof (char*));
    args[0] = str1;
    int n_spaces = 1;
    char* p = strtok(str2, " ");
    while (p) 
    {
        args = realloc (args, sizeof (char*) * ++n_spaces);

        if (args == NULL)
            exit (-1); /* memory allocation failed */

        args[n_spaces-1] = p;
        p = strtok (NULL, " ");
    }
    args = realloc (args, sizeof (char*) * ++n_spaces);
    args[n_spaces-1] = p; // args[n_spaces-1] = NULL

    short int isBackgroundJob = -1;
    if(strcmp(args[n_spaces-2], "&")==0)
    {
        isBackgroundJob = 1;
        args[n_spaces - 2] = NULL;
    }
    else if(args[n_spaces-2][strlen(args[n_spaces-2])-1]=='&')
    {
        isBackgroundJob = 1;
        args[n_spaces-2][strlen(args[n_spaces-2])-1] = '\0';
    }


    pid_t childPid = fork();
	if (childPid == 0)
    {
        setpgid(0, 0);
        //printf("Child initial group id - %d\n", getpgrp());
        if(args[1]!=NULL){
            if(args[1][0]=='~')
            {
                args[1] = ret_path(args[1]);
            }
        }
	    int x = execvp(args[0], args); //calls execvp  
        if(x<0)// error
        {
            //printf("%s : No such command found\n", args[0]);
            perror("Error");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
	}
    else 
    {
		if (isBackgroundJob>0)
        {
            printf("[%d] initiated\n", childPid);
            free(args);
            return childPid;
		} 
        else 
        {
            int stat;
			wait(&stat);
        }
        free(args);
        return 0;	
    }    
}