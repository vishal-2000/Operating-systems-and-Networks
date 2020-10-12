// This program enables merge sort to use concurrency while merging the array
// We use threads here to perform concurrent merge sort
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

//#include<sys/wait.h> 
//#include <sys/types.h>

//#include <sys/types.h> 
#include <unistd.h>
#include <pthread.h>


void* merge_sort(void* a);
void merge_arr(int* arr, int l, int mid, int r);
void* selection_sort(void* a);

typedef struct THREAD_STRUCT{
    int l, r, *arr;
} thread_struct;

int main()
{
    // Measuring time
    clock_t init_seconds = clock();
    //
    int n;
    scanf("%d", &n);
    int arr[n+1];
    for(int i=0;i<n;i++){
        scanf("%d", &arr[i]);
    }

    pthread_t tid;
    thread_struct a;
    a.l = 0;
    a.r = n-1;
    a.arr = arr;

    if(pthread_create(&tid, NULL, merge_sort, &a) < 0){
        perror("pthread_create Error");
        exit(EXIT_FAILURE);
    }
    pthread_join(tid, NULL);

    // Printing Array
    /*printf("Sorted List:\n");
    for(int i=0;i<n;i++)
        printf("%d  ", arr[i]);
    printf("\n");*/


    clock_t end_seconds = clock();
    printf("Time taken for concurrent execution:%ld\n", end_seconds - init_seconds);
    return 0;   
}

void* merge_sort(void* a){

    thread_struct *args = (thread_struct*) a;
    int l = args->l;
    int r = args->r;
    int* arr = args->arr;
    int mid = l + (r-l)/2;
    
    //sort left half array
    thread_struct a1;
    a1.l = l;
    a1.r = mid;
    a1.arr = arr;
    pthread_t tid1;
    int er;
    if(mid-l+1 < 5)
        er = pthread_create(&tid1, NULL, selection_sort, &a1);
    else
        er = pthread_create(&tid1, NULL, merge_sort, &a1);
    if(er < 0){
        perror("pthread_create Error");
        exit(EXIT_FAILURE);
    }

     //sort right half array
    thread_struct a2;
    a2.l = mid+1;
    a2.r = r;
    a2.arr = arr;
    pthread_t tid2;
    if(r-mid < 5)
        er = pthread_create(&tid2, NULL, selection_sort, &a2);
    else 
        er = pthread_create(&tid2, NULL, merge_sort, &a2); 
    if(er < 0){
        perror("pthread_create Error");
        pthread_join(tid1, NULL);
        exit(EXIT_FAILURE);
    }

     //wait for the two halves to get sorted
     pthread_join(tid1, NULL);
     pthread_join(tid2, NULL);

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

void* selection_sort(void* a){

    thread_struct *args = (thread_struct*) a;
    int l = args->l;
    int r = args->r;
    int* arr = args->arr;
    int mid = l + (r-l)/2;

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