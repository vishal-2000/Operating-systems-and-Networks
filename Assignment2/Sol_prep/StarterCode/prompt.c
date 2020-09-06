#include "prompt.h"
#include "headers.h"
#include "sys/utsname.h"

extern char* root;

struct utsname unameData;

void prompt() 
{
    uname(&unameData);   
    char *buf = NULL, buf2[4096];
    buf=getlogin();
    getcwd(buf2, 4096); // getting current directory
    printf("%s@%s:",buf, unameData.nodename); // because it is mentioned that we need to get hostname
    if(strlen(buf2) < strlen(root))
    {
        printf("%s>", buf2);
        return;
    }
    for(int i=0; i<strlen(root); i++)
    {
        if(buf2[i]!=root[i])
        {
            printf("%s>", buf2);
            return;
        }
    }
    printf("~");
    for(int i=strlen(root);i<strlen(buf2);i++)
    {
         printf("%c", buf2[i]);
    }
    printf(">");
}