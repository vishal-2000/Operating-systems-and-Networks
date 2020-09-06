#include "headers.h"
#include "echo_com.h"

void echo_com(char *str)
{
    char *temp1;
    char* token = strtok_r(str, " ", &temp1);
    while(token!=NULL)
    {
        printf("%s ", token);
        token = strtok_r(temp1, " ", &temp1);
    }
    printf("\n");
}