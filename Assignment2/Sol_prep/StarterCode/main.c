#include "prompt.h"
#include "headers.h"
#include "unistd.h" // for getcwd()

char *root;

int main()
{
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
        

        

        free(line);
    }
}
