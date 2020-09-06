#include "headers.h"
#include "changedir.h"

extern char* root;

void changedir(char* str)
{
    char buf[4096];
    if(str==NULL){
        sprintf(buf, "%s", root);
    }
    else if(str[0]=='~')
    {
        str = str + 1;
        sprintf(buf,"%s%s", root, str);
    }
    else if(strcmp(str, ".")==0)
    {
        return;
    }
    else if(strcmp(str, "..")==0)
    {
        sprintf(buf, "%s", str);
    }
    else{
        sprintf(buf, "%s", str);
    }
    int x = chdir(buf);
    if(x<0)
    {
        perror("Error");
    }
}