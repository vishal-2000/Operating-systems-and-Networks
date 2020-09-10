#include "headers.h"
#include "dirent.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>// for file owner getpwuid
#include <grp.h>// for group owner
#include <time.h> // for converting time to %s

void longform(char* str, int f);
void normalform(char* str, int f);
void printPerm(long long int x);
long long int decToBinary(int n);
char* ret_path(char* str);

struct stat statbuf;

extern char* root;

void ls_com(char *str)
{
    char flags[4][5] = {"-l", "-a", "-la", "-al"};
    int longf = -1, hidf = -1;

    char* S[PATH_MAX];
    int n_of_tokens=0;

    char *temp1;
    char* token = strtok_r(str, " ", &temp1);
    while(token!=NULL)
    {
        int parflag = -1;
        for(int i=0;i<4;i++)
        {
            if(strcmp(token, flags[i])==0)
            {
                parflag = 0;
                if(i==0)
                    longf = 0;
                else if(i==1)
                    hidf = 0;
                else
                {
                    longf = 0;
                    hidf = 0;
                }
            }
        }
        if(parflag == 0)
        {
            token = strtok_r(temp1, " ", &temp1);
            continue;
        }
        
        S[n_of_tokens] = token;
        n_of_tokens++;
        
        token = strtok_r(temp1, " ", &temp1);
    }
    if(n_of_tokens==0)
    {
        n_of_tokens++;
        S[0] = ".";
    }

    for(int i=0;i<n_of_tokens;i++)
    {
        if(n_of_tokens>1)
            printf("%s:\n", S[i]);
        
        char* final_path = ret_path(S[i]);

        if(longf==0)
            longform(final_path, hidf);
        else
            normalform(final_path, hidf);
        free(final_path);
    } 
    return;
}

char* ret_path(char* str)
{
    char* buf;
    buf = (char*)malloc(4096);
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
        sprintf(buf, "%s", str);
    }
    else if(strcmp(str, "..")==0)
    {
        sprintf(buf, "%s", str);
    }
    else{
        sprintf(buf, "%s", str);
    }
    //free(str);
    return buf;
}

void normalform(char* str, int f)
{
    struct dirent **namelist;
    int n;

    n = scandir(str, &namelist, NULL, alphasort);
    if (n == -1) {
            perror("scandir");
            return;
    }

    for(int i=0;i<n;i++) {
        if(f==-1 && namelist[i]->d_name[0]=='.')
        {
            free(namelist[i]);
            continue;
        }
        printf("%s\n", namelist[i]->d_name);
        free(namelist[i]);
    }

    free(namelist);
    //printf("\n");
    //exit(EXIT_SUCCESS);
}

void longform(char* str, int f)
{
    struct dirent **namelist;
    int n;

    n = scandir(str, &namelist, NULL, alphasort);
    if (n == -1) {
            perror("scandir");
            return;
    }

    long int maxi = 0;
    long long int total = 0;
    for(int i=0;i<n;i++){
        stat(namelist[i]->d_name, &statbuf);
        if(namelist[i]->d_name[0]!='.'){
            long long int m = statbuf.st_size;
            long long int k = m/4096;
            if(k*4096!=m)
                k++;
            total = total + 4*k;
        }
        if(statbuf.st_size > maxi)
            maxi = statbuf.st_size;
    }
    printf("Total = %lld\n", total);
    int digits = 0;
    while(maxi!=0)
    {
        digits++;
        maxi/=10;
    }

    for(int i=0;i<n;i++) {
        if(f==-1 && namelist[i]->d_name[0]=='.')
        {
            free(namelist[i]);
            continue;
        }
        stat(namelist[i]->d_name, &statbuf);
        // Printing process
        if(S_ISDIR(statbuf.st_mode))
            printf("d");
        else
            printf("-");
        printPerm(decToBinary(statbuf.st_mode));
        // Ends Here
        printf("%ld %s %s",statbuf.st_nlink, getpwuid(statbuf.st_uid)->pw_name, getgrgid(statbuf.st_gid)->gr_name);
        long int t = statbuf.st_size, u = 0;
        while(t!=0)
        {
            u++;
            t=t/10;
        }
        while(u<digits)
        {
            u++;
            printf(" ");
        }
        printf(" %ld",statbuf.st_size);
        char *Temp = asctime(localtime(&statbuf.st_mtime));
        Temp[strlen(Temp)-1] = '\0';
        //strftime(Temp,200, "%d.%m.%Y %H:%M:%S", localtime(&statbuf.st_mtime));
        printf(" %s", Temp);
        printf(" %s\n", namelist[i]->d_name);
        free(namelist[i]);
    }

    free(namelist);
    //printf("\n");
}

void printPerm(long long int x)
{
    int perm[13];
    for(int i=0;i<12;i++)
    {
        perm[11-i] = x%10;
        x/=10;
    }
    for(int i = 3;i <= 11;i++)
    {
        if(perm[i]==0)
            printf("-");
        else
            printf("r");
        i++;
        if(perm[i]==0)
            printf("-");
        else
            printf("w");
        i++;
        if(perm[i]==0)
            printf("-");
        else
            printf("x");
    }
    printf(" ");
}

long long int decToBinary(int n) 
{ 
      
    // array to store octal number 
    long long int binaryNum = 0; 
  
    // counter for octal number array 
    long long int i = 1; 
    while (n != 0) { 
  
        // storing remainder in octal array 
        binaryNum = binaryNum + (n % 2)*i; 
        n = n / 2; 
        i*=10; 
    } 
    return binaryNum;
} 

