#include<stdio.h> 
#include<stdlib.h>
#include<fcntl.h> 
#include<errno.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/file.h> 
#include <unistd.h> 
#include <string.h>
#include <time.h> 

int main(int argc, char *argv[]){
    int fd1 = open(argv[1], O_RDONLY);
    if (fd1==-1)
    {
        perror("Error");
        exit(errno);
    }
    else
    {
        mkdir("Assignment", 0700); // Read write and execute permissions only to the user
        char pathFile[300];
        sprintf(pathFile, "Assignment//%s", argv[1]);
        int fd2 = open(pathFile, O_RDWR | O_CREAT , 0600); // Created a file with same name inside "Assignment" dir
        if(fd2==-1)
        {
            perror("Error");
            exit(errno);
        }
        long long int offset = lseek(fd1, 0, 2);
        double inp_size = offset + 1;
        double percentage_completed = 0.0;
        lseek(fd2, 0, 0);
        char c1[1000009], c2[1000009];// temporary storage/ buffer
        //long long int prevpos = offset;
        int si = 1000000;
        time_t seconds;
        seconds = time(NULL); 
        while(offset > 0){
            while(offset < si){
                si = si/10;
            }
            offset = lseek(fd1, -1*si, 1);
            read(fd1, c1, si);
            for(int i=si-1;i>=0;i--)
            {
                c2[si - 1 - i] = c1[i];
            }
            write(fd2, c2, si);
            lseek(fd1, -1*si, 1);
            percentage_completed = ((inp_size - offset)*100)/inp_size;
            char tmp[8]={0x0};
            sprintf(tmp,"%3.2lf", percentage_completed);
            fflush(stdin); 
            system("clear"); 
            write(1, tmp, sizeof(tmp));
        }
        close(fd2);
        /*seconds = time(NULL) - seconds;
        char t[12]={0x0};
        sprintf(t,"%11ld", seconds);
        write(1, t, sizeof(t));
        close(fd2);*/
    }
    close(fd1);
    write(1, "\n", 1);
    return 0;
}