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
    arr = (int*)malloc(sizeof(int)*n);

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

    clock_t end_seconds = clock();
    printf("Time taken for concurrent execution:%ld\n", end_seconds - init_seconds);
    return 0;   
}

void merge_sort(int* arr, int l, int r){
    int mid = l + (r-l)/2;
    if(mid-l+1 < 5)
        selection_sort(arr, l, mid);
    else
        merge_sort(arr, l, mid);
    if(r-mid < 5)
        selection_sort(arr, mid+1, r);
    else
        merge_sort(arr, mid+1, r);
    merge_arr(arr, l, mid, r);
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