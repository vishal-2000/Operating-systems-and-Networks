// This program enables merge sort to use concurrency while merging the array
// We use processes here to perform concurrent merge sort
// We will use the concept of shared memory to get this sorting done
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <time.h>

#include<sys/wait.h> 
#include <sys/types.h>

#include <sys/types.h> 
#include <unistd.h>


void merge_sort(int* arr, int l, int r);
void merge_arr(int* arr, int l, int mid, int r);
void selection_sort(int* arr, int l, int r);

int main()
{
    // Measuring time
    clock_t init_seconds = clock();
    //
    int n;
    scanf("%d", &n);
    int* arr;
    size_t size_shared_mem = sizeof(int)*n;
    // We will now create a shared memory segment
    key_t key = IPC_PRIVATE;
    int sh_id = shmget(key, size_shared_mem, IPC_CREAT | IPC_EXCL | 0666);  //A key value of IPC_PRIVATE requests a new shared memory segment without an associated key 
    if(sh_id < 0){
        perror("Shared memory Error");
        exit(1);
    }
    // Memory segment created with identifier "sh_id"
    arr = shmat(sh_id, NULL, 0); // Attaching the created segment to this process (main process)
    // 2nd Arguement = NULL, because we want it to choose the page that is free all on it's own
    if(arr == (int *)-1){
        perror("Shared Memory Attachment Error");
        exit(1);
    }
    for(int i=0;i<n;i++){
        scanf("%d", &arr[i]);
    }

    merge_sort(arr, 0, n-1);

    // Printing Array
    /*
    printf("Sorted List:\n");
    for(int i=0;i<n;i++)
        printf("%d  ", arr[i]);
    printf("\n");
    */

    // Detaching the shared segment from this process
    if(shmdt(arr) < 0){
        perror("Error while detaching the segment");
        exit(1);
    }

    // Deleting the shared segment
    if(shmctl(sh_id, IPC_RMID, NULL) < 0){
        perror("Error while deleting shared segment");
        exit(1);
    }
    clock_t end_seconds = clock();
    printf("Time taken for concurrent execution:%ld\n", end_seconds - init_seconds);
    return 0;   
}

void merge_sort(int* arr, int l, int r){
    int mid = l + (r-l)/2;
    int ch1, ch2;
    ch1 = fork();
    if(ch1 < 0){
        perror("Failed to create a child process(Left)");
        exit(-1);
    }
    else if(ch1==0){ // Left child
        if((mid - l + 1) <= 5){
            selection_sort(arr, l, mid);
        }
        else{
            merge_sort(arr, l, mid);
        }
        exit(1);
    }
    else{ // Back to parent again
        ch2 = fork();
        if(ch2 < 0){
            perror("Failed to create a child process(Left)");
            exit(-1);
        }
        else if(ch2==0){ // Right child
            if((r-mid) <= 5){
                selection_sort(arr, mid+1, r);
            } 
            else{
                merge_sort(arr, mid+1, r);
            }
            exit(1);
        }
        else{
            int stat;
            waitpid(ch1, &stat, WUNTRACED);
            waitpid(ch2, &stat, WUNTRACED);
            merge_arr(arr, l, mid, r);
        }
    }
}

void merge_arr(int* arr, int l, int mid, int r){
    int n1 = mid - l+1;
    int n2 = r-mid;
    int L[n1], R[n2];
    for(int i=0;i<n1;i++)
        L[i] = arr[i + l];
    for(int i=0;i<n2;i++)
        R[i] = arr[mid+1+i];
    
    int i = 0, j=0, k = l;
    while(i< n1 && j < n2){
        if(L[i] < R[j]){
            arr[k] = L[i];
            k++;
            i++;
        }
        else{
            arr[k] = R[j];
            k++;
            j++;
        }
    }
    while(i< n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while(j< n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

void selection_sort(int* arr, int l, int r){
    int i, j, min_index;  
    for (i = l; i < r; i++) 
    {  
        min_index = i; 
        for (j = i+1; j <= r; j++) 
          if (arr[j] < arr[min_index]) 
            min_index = j; 
  
        int temp = arr[min_index];
        arr[min_index] = arr[i];
        arr[i] = temp; 
    }
}