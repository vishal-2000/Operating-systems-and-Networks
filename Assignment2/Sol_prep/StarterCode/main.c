#include "prompt.h"
#include "headers.h"
#include "unistd.h" // for getcwd()
#include "string.h"

char *root;

void changedir(char* str);

int main()
{
    char command[3][6] = {"cd", "pwd", "echo"};
    printf("\e[1;1H\e[2J"); // System() is prohibited hence used regex
    root =(char *)malloc(4096);

    getcwd(root, 4096);
    while (1)
    {
        prompt();
        char *line = NULL;
        size_t len = 0;
        ssize_t read = -1;
        while(read==-1)
        {
            read = getline(&line, &len, stdin); // len gives sizeof(line) and remember that line also contains '\n' at the end
        }
        line[read-1] = '\0';
        //printf("%s, %ld, %ld\n", line, len, read);
        char *temp1;
        char* token = strtok_r(line, "&", &temp1);
        while(token!=NULL)
        {
            char* temp2;
            char* subtoken = strtok_r(token, " ", &temp2);
            if(strcmp(subtoken, command[0])==0)
            {
                subtoken = strtok_r(temp2, " ", &temp2);
                changedir(subtoken);
            }
            //free(temp2);
            //free(subtoken);
            token = strtok_r(temp1, "&", &temp1);
        }
        //free(token);
        free(line);
    }
}

void changedir(char* str)
{
    char buf[4096];
    if(str==NULL){
        return;
    }
    if(str[0]=='~')
    {
        if(strlen(str)==1){
            return;
        }
        str = str + 1;
        sprintf(buf,"%s%s", root, str);
    }
    else if(str[0]=='.' && str[1]=='.'){
        char* cur;
        getcwd(cur, 4096);
        if(strcmp(cur, root)==0){
            printf("%s\n", root);
            return;
        }
       sprintf(buf, "%s", str);
    }
    else{
        sprintf(buf, "%s", str);
    }
    //printf("%s\n", buf);
    int x = chdir(buf);
    if(x<0)
    {
        perror("Error:");
    }
}