#include <stdio.h>
#include <stdlib.h>
int main()
{
    int n;
    scanf("%d", &n);
    printf("%d\n", n);
    for(int i=0;i<n;i++){
        printf("%d\n", rand());
    }
    return 0;
}