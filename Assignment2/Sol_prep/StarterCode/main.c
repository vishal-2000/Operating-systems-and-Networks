#include "prompt.h"
#include "changedir.h"
#include "echo_com.h"
#include "headers.h"

char *root;

void changedir(char* str);
void echo_com(char* str);
void ls_com(char* str);

int main()
{
    char command[4][6] = {"cd", "pwd", "echo", "ls"};
    printf("\e[1;1H\e[2J"); // System() is prohibited hence used regex
    root =(char *)malloc(4096);

    getcwd(root, 4096);
    while (1) // Main Execution Loop (Analogous to kernel)
    {
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
        char* token = strtok_r(line, "&", &temp1);
        while(token!=NULL)
        {
            char* temp2;
            char* subtoken = strtok_r(token, " ", &temp2);
            //printf("Subtoken: %s\n", subtoken);
            if(strcmp(subtoken, command[0])==0) // cd command
            {
                subtoken = strtok_r(temp2, " ", &temp2);
                changedir(subtoken);
            }
            else if(strcmp(subtoken, command[1])==0) // pwd command
            {
                char presentdir[4096];
                getcwd(presentdir, 4096);
                printf("%s\n", presentdir);
            }
            else if(strcmp(subtoken, command[2])==0) // echo command
            {
                echo_com(temp2);
            }
            else if(strcmp(subtoken, command[3])==0)
            {
                ls_com(temp2);
            }
            
            token = strtok_r(temp1, "&", &temp1);
        }
        if(line!=NULL)
            free(line);
    }
}