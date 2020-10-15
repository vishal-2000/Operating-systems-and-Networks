#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h> // for pthread_kill

#include <time.h>

// -----------------------------------------------------------------> struct definitions
typedef struct pharm_thread_struct{
    int pharma_com_num;
    float suc_prob;
    int batches_running;
    // batches running - no of batches that are still being used by vaczones
    int vac_per_batch;
}pharma_thread_struct;
typedef struct vac_zone_thread_struct{
    int col_num;
}vaczone_thread_struct;
typedef struct student_thread_struct{
    int stud_num;
}stud_thread_struct;

static int terminate = 0; // If 0--> false, 1--> true;

// ---------------------------------------------------------->  mutex locks intialization
pthread_mutex_t lock_pharma_com, lock_vac_zon, lock_stud;

// ---------------------------------------------------------------> function declarations
void* pharma_com(void* a);
int initiate_prog(int n, int m, int o);

int main()
{
    srand(time(NULL));
    int n, m, o; //n=#pharma, m=#vac_zones, o=#students
    scanf("%d %d %d", &n, &m, &o); // -------------> Taking n, m, o as input
    if(initiate_prog(n, m, o) < 0)
        return 0;
    pharma_thread_struct p[n+2];
    vaczone_thread_struct q[m+2];
    stud_thread_struct r[o+2];
    for(int i=0;i<n;i++){ // ----------------------> Taking success prob. as input
        scanf("%d", &p[i].suc_prob);
    }

    /* ids of each of the threads will be stored for unique identification*/
    pthread_t pharm_id[n+2], vac_zon[m+2], stud[o+2]; 

    // ------------------------------------------>   Create pharma companies
    for(int i=0;i<n;i++){
        p[i].pharma_com_num = i+1;
        p[i].batches_running = 0;
        p[i].vac_per_batch = 0;
        if(pthread_create(&pharm_id[i], NULL, pharma_com, &p[i]) < 0){
            perror("pthread ERROR");
            for(int j=0;j<i;j++){
                pthread_kill(pharm_id[j], 1);
            }
            return 0;
        }
    }// -----------------------------------------> end creation

    // ---------------------------------------------------> Join the threads
    for(int i=0;i<n;i++){
        pthread_join(pharm_id[i], NULL);
        printf("Thread %d joined!\n", i+1);
    }
    // -----------------------------------------------------> Threads Joined
    return 0;
}

void* pharma_com(void* a){
    pharma_thread_struct *args = (pharma_thread_struct*) a;
    while(1){
        pthread_mutex_lock(&lock_pharma_com);
        if(terminate==0)
            break;
        pthread_mutex_unlock(&lock_pharma_com);
        while(args->batches_running>0);
        pthread_mutex_lock(&lock_pharma_com);
        args->batches_running = rand()%5 + 1; // batches from 1 to 5
        args->vac_per_batch = rand()%20 + 1;
        pthread_mutex_unlock(&lock_pharma_com);
        sleep(rand()%4 + 2);
        // batches_running is zero

    }


    /*
    pharma_thread_struct *args = (pharma_thread_struct*) a;
    pthread_mutex_lock(&lock_pharma_com);
    printf("Thread %d is running!\n", args->pharma_com_num);
    pthread_mutex_unlock(&lock_pharma_com);
    sleep((rand()%10 + 1)*2);
    pthread_mutex_lock(&lock_pharma_com);
    printf("Thread %d comleted it's task!\n", args->pharma_com_num);
    pthread_mutex_unlock(&lock_pharma_com);
    */
}

int initiate_prog(int n, int m, int o){
    int max_threads = 100;
    if(n+m+o > max_threads){
        printf("The input crossed the max thread limit, not allowed to simulate\n");
        return -1;
    }

    if (pthread_mutex_init(&lock_pharma_com, NULL) != 0) { // initializing the lock
        printf("\n mutex init has failed\n"); 
        return -1; 
    }
    if (pthread_mutex_init(&lock_vac_zon, NULL) != 0) { // initializing the lock
        printf("\n mutex init has failed\n"); 
        return -1; 
    }
    if (pthread_mutex_init(&lock_stud, NULL) != 0) { // initializing the lock
        printf("\n mutex init has failed\n"); 
        return -1; 
    } 
}