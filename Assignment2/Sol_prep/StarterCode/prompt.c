#include "prompt.h"
#include "headers.h"
#include "sys/utsname.h"
#include "unistd.h"
#include "string.h"

extern char* root;

struct utsname unameData;

void prompt() 
{
    uname(&unameData);
    //printf("%s@%s:~>", unameData.nodename,unameData.sysname);    
    char *buf = NULL, buf2[4096];
    buf=getlogin();
    getcwd(buf2, 4096);
    //printf("Current Directory : %s\n", buf2);
    printf("%s@%s:~",buf, unameData.nodename); // because it is mentioned that we need to get hostname
    for(int i=strlen(root);i<strlen(buf2);i++)
    {
         printf("%c", buf2[i]);
    }
    printf(">");
    //free(buf);
}
