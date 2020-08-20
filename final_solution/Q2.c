#include <dirent.h> 
#include <string.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<errno.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/file.h> 
#include <unistd.h> 

#include <stdio.h>

struct stat statbuf, statbuf1, statbuf2;

int reversed(char* f1, char* f2);
void directoryPermissions(char* f, char*c2);
int decToOctal(int n); 

int main(int argc, char *argv[])
{
    stat(argv[3], &statbuf);
    char* cy = "Yes\n";
    char* cn= "No\n";
    if(S_ISDIR(statbuf.st_mode) == 0)
    {
        //perror("Error Directory");
        char *c_1 = "Directory is created: No\n";
        write(1, c_1, strlen(c_1));
        
    }
    else
    {
        char *c_1 = "Directory is created: Yes\n";
        write(1, c_1, strlen(c_1));

        // Permissions of directory
        char* c_2 = "Directory";
        directoryPermissions(argv[3], c_2);
        

    }
    // checking if the files contain reversed content

    char* c1 = "Old file";
    char* c2 = "New file";

    stat(argv[2], &statbuf2);// stores info regarding old file
    stat(argv[1], &statbuf1);// stores info regarding new file
    if(statbuf1.st_mode==0 && statbuf2.st_mode==0)
    {
        if(statbuf2.st_mode==0){
            int u = open(argv[2], O_RDONLY);
            perror("Error Old file");
            close(u);
        }
        if(errno = statbuf1.st_mode==0){
            int u = open(argv[1], O_RDONLY);
            perror("Error New file");
            close(u);
        }
        exit(errno);
    }    
    else if(statbuf2.st_mode==0)
    {
        perror("Error Old file");
        directoryPermissions(argv[1], c2);
    }
    else if(statbuf1.st_mode==0)
    {
        perror("Error New file");
        directoryPermissions(argv[2], c1);
    }
    else{
        if(reversed(argv[2], argv[1])==1){
            char* c_2 = "Whether file contents are reversed in newfile: Yes\n";
            write(1, c_2, strlen(c_2));
        }
        else{
            char* c_2 = "Whether file contents are reversed in newfile: No\n";
            write(1, c_2, strlen(c_2));
        }
        directoryPermissions(argv[2], c1);
        directoryPermissions(argv[1], c2);
    }
    return 0;
}
// checking file reversals ----------------------------------------------------------->
int reversed(char* f1, char* f2){
    int fd1 = open(f1, O_RDONLY);
    if (fd1==-1)
    {
        perror("Old file Error: ");
        return 0;
    }
    else
    {
        char pathFile[300];
        sprintf(pathFile, "%s", f2);
        int fd2 = open(pathFile, O_RDONLY);
        if (fd2==-1)
        {
            perror("New file Error:");
            return 0;
        }
        long long int offset = lseek(fd1, 0, 2);
        lseek(fd2, 0, 0);
        char c1[1000009], c2[1000009];// temporary storage/ buffer
        int si = 1000000;
        while(offset > 0){
            while(offset < si){
                si = si/10;
            }
            offset = lseek(fd1, -1*si, 1);
            read(fd1, c1, si);
            read(fd2, c2, si);
            for(int i=si-1;i>=0;i--)
            {
                if(c1[i]!=c2[si-i-1])
                {
                    close(fd1);
                    close(fd2);
                    return 0;
                }
            }
            lseek(fd1, -1*si, 1);
        }
        if(lseek(fd1, 0, 2)!=lseek(fd2, 0, 2)) // edge case where size(fd2)!=size(fd1)
        {
            close(fd1);
            close(fd2);
            return 0;
        }
        close(fd2);
    }
    close(fd1);
    return 1;
}
// Printing permissions ------------------------------------------------------------->
void directoryPermissions(char* f, char*c2)
{
    stat(f, &statbuf);
    char* c3 = ": Yes\n";
    char* c4 = ": No\n";
    // User permissions
    int octal_code = decToOctal(statbuf.st_mode);
    int perm[3];
    for(int i=0;i<3;i++)
    {
        perm[2-i] = octal_code%10;
        octal_code/=10;
    }
    if(perm[0]>=4){
        char* c1 = "User has read permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "User has read permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }
    if(perm[0]==2||perm[0]==3||perm[0]==6||perm[0]==7){
        char* c1 = "User has write permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "User has write permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }
    if(perm[0]==1||perm[0]==3||perm[0]==5||perm[0]==7){
        char* c1 = "User has execute permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "User has execute permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }


    // Group permissions
    if(perm[1]>=4){
        char* c1 = "Group has read permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "Group has read permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }
    if(perm[1]==2||perm[1]==3||perm[1]==6||perm[1]==7){
        char* c1 = "Group has write permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "Group has write permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }
    if(perm[1]==1||perm[1]==3||perm[1]==5||perm[1]==7){
        char* c1 = "Group has execute permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "Group has execute permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }

    // Others permissions
    if(perm[2]>=4){
        char* c1 = "Others has read permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "Others has read permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }
    if(perm[2]==2||perm[2]==3||perm[2]==6||perm[2]==7){
        char* c1 = "Others has write permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "Others has write permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }
    if(perm[2]==1||perm[2]==3||perm[2]==5||perm[2]==7){
        char* c1 = "Others has execute permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c3, strlen(c3));
    }
    else{
        char* c1 = "Others has execute permissions on ";
        write(1, c1, strlen(c1));
        write(1, c2, strlen(c2));
        write(1, c4, strlen(c4));
    }

}
// Decimal to octal conversion ---------------------------------------------------->
int decToOctal(int n) 
{ 
      
    // array to store octal number 
    int octalNum = 0; 
  
    // counter for octal number array 
    int i = 1; 
    while (n != 0) { 
  
        // storing remainder in octal array 
        octalNum = octalNum + (n % 8)*i; 
        n = n / 8; 
        i*=10; 
    } 
    
    return octalNum;
} 