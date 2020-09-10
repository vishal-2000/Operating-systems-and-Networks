#include <sys/types.h> 
#include <sys/wait.h>
#include "prompt.h"
#include "run_builtin.h"
#include "run_syscom.h"
#include "changedir.h"
#include "echo_com.h"
#include "ls_com.h"
#include "headers.h"

#include<fcntl.h>

char *root;

int run_syscom(char* str1, char* str2);
void run_builtin(char* str, char* str2);
void changedir(char* str);
void echo_com(char* str);
void ls_com(char* str);

int main()
{
    int bgPIDs[500];
    int bgnum = 0;
    char* proc_names[500]; // stores the names of background process

    char command[5][6] = {"cd", "pwd", "echo", "ls", "pinfo"};
    printf("\e[1;1H\e[2J"); // System() is prohibited hence used regex
    root =(char *)malloc(PATH_MAX);

    getcwd(root, PATH_MAX);
    while (1) // Main Execution Loop (Analogous to kernel)
    {   
        int stat;
        
        pid_t childpid = waitpid(-1, &stat, WNOHANG | WUNTRACED);
        if(childpid > 0)
        {
            char* token = NULL;
            int x = (bgnum<=500)?bgnum:500;
            for(int i=0;i<bgnum;i++)
            {
                if(childpid==bgPIDs[i])
                {
                    token = proc_names[i];
                }
            }
            if(WIFEXITED(stat)!=0)
            {
                printf("%s with pid = [%d] exited normally!\n", token, childpid);
            }
            else if(WIFSTOPPED(stat)!=0)
            {
                printf("%s with pid = [%d] stopped!\n", token, childpid);
            }
            else if(WIFSIGNALED(stat)!=0)
            {
                printf("%s with pid = [%d] Terminated by the signal %d\n", token, childpid, WTERMSIG (stat));
            }
        }

        prompt();
        char *line = NULL;
        size_t len = 0;
        ssize_t read = -1;
        while(read==-1)
        {
            read = getline(&line, &len, stdin); // len gives sizeof(line) and remember that line also contains '\n' at the end
        }
        line[read-1] = '\0'; // removing next line character
        read--; // Gives the length of line excluding '\n'
        char *temp1;
        char* token = strtok_r(line, ";", &temp1);
        while(token!=NULL)
        {
            char* temp2;
            char* subtoken = strtok_r(token, " ", &temp2);
            short int is_builtin = -1;
            for(int j=0;j<5;j++)
            {
                if(strcmp(subtoken, command[j])==0)
                    is_builtin = 1;
                    
            }
            if(is_builtin>0)
            {
                run_builtin(subtoken, temp2);
            }
            else
            {
                char tempstor[PATH_MAX];
                sprintf(tempstor, "%s %s", subtoken, temp2);
                int x = run_syscom(subtoken, temp2);
                if(x>0){
                    bgPIDs[bgnum%500] = x;
                    if(bgnum>=500){
                        free(proc_names[bgnum%500]);
                    }
                    proc_names[bgnum%500] = (char*)malloc(sizeof(tempstor));
                    strcpy(proc_names[bgnum], tempstor);
                    bgnum++;
                }

            }
            token = strtok_r(temp1, "&", &temp1);
        }
        if(line!=NULL)
            free(line);
    }
}