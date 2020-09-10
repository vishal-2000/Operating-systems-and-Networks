#include "headers.h"
//#include "pinfo_com.h"
extern char* root;

void path_to_com(char* s);

void pinfo_com(char* arg)
{

    int pid = 0;
    //printf("Hi\n");
    for(int i = 0; i<strlen(arg); i++)
    {
        pid = pid*10 + arg[i] - '0';
    }
    printf("pid--%d\n", pid);
    char path[PATH_MAX];
    char buf[PATH_MAX];
    sprintf(path, "/proc/%d/status", pid);
    FILE* fp = fopen(path, "r");
    if(fp==NULL) {
        printf("%s\n", path);
        perror("Error while opening the proc/pid/status file\n"); 
    }
    else
    {
        char state[PATH_MAX];
        fgets(buf,256, fp);
        //printf("%s", buf);
        fgets(buf, 256, fp);
        //printf("%s", buf);
        fgets(state, 256, fp);
        //sscanf(buf, "State:\t%c", &state);
        printf("Process Status--%c%c\n", state[7], state[8]);
        fclose(fp);
    }

    sprintf(path, "/proc/%d/statm", pid);
    fp = fopen(path, "r");
    if(!fp) 
    {
        perror("Error while opening the proc/pid/statm file\n");
    }
    else 
    {
        int len = 0;
        char c;
        while((c=fgetc(fp))!=' ') 
            buf[len++] = c;
        buf[len] = '\0';
        printf("Memory -- %s\n", buf);
        fclose(fp);
    }

    sprintf(path, "/proc/%d/exe", pid);
    int len = -1;
    len = readlink(path, buf, PATH_MAX - 1);
    if(len==-1) {
        perror("Error while opening the proc/pid/exe file\n");
    }
    else {
        buf[len] = '\0';
        path_to_com(buf);
        printf("Executable Path -- %s\n", buf);
    }
}

void path_to_com(char* str){
    char buf[PATH_MAX];
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
    str = buf;
}