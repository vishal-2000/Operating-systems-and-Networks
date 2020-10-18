#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h> // for pthread_kill

#include <time.h>

// -----------------------------------------------------------------> struct definitions
typedef struct pharm_thread_struct{
    int pharma_com_num;
}pharma_thread_struct;
typedef struct vac_zone_thread_struct{
    int zone_num;
}vaczone_thread_struct;
typedef struct student_thread_struct{
    int stud_num;
}stud_thread_struct;

// Global variables
int terminate; // If 0--> false, 1--> true;
int *batches_ready, *batches_runnin, *vac_per_batch;
float *succ_proba_pharma, *succ_proba_vaczon;
int n, m, o; //n=#pharma, m=#vac_zones, o=#students
// ----------------------------------------------------------> queue maintaining free vac slots
// ----> queue is used to implement randomized slot allotment to students
//int queue_free_slots[10000]; // can store 10000 slots at once
//int pop();
//int push(int x);
int no_of_studs_waiting; //#students waiting at the gate
int *slots_available; // available slots of each vac zone
int *slots_filled;
short int* flags_between_studs_vaczones;
//static int batches_ready_for_distribution;
//static int slots_ready_for_allotment;

// ---------------------------------------------------------->  mutex locks and conditional variables
pthread_mutex_t lock_pharma_com, lock_vac_zon, lock_stud, lock_screen, lock_rand, lock_wait_stud, lock_slot_update;

// ---------------------------------------------------------------> function declarations
int initiate_prog(int n, int m, int o);
void* pharma_com(void* a);
void* vaccination_zone(void* a);
void* student(void* a);
int minimum(int a, int b, int c);

int main()
{
    srand(time(NULL));
    scanf("%d %d %d", &n, &m, &o); // -------------> Taking n, m, o as input
    if(initiate_prog(n, m, o) < 0)
        return 0;
    pharma_thread_struct p[n+2];
    vaczone_thread_struct q[m+2];
    stud_thread_struct r[o+2];
    for(int i=0;i<n;i++){ // ----------------------> Taking success prob. as input
        scanf("%f", &succ_proba_pharma[i]);
    }

    /* ids of each of the threads will be stored for unique identification*/
    pthread_t pharm_id[n+2], vac_zon_id[m+2], stud_id[o+2]; 

    printf("\033[1;32m"); // Bold green color 
    printf("Simulation started!\n");

    // ------------------------------------------>   Create pharma companies
    for(int i=0;i<n;i++){
        p[i].pharma_com_num = i; // pharma com ids from 0 to n-1 (output will show 1 to n)
        if(pthread_create(&pharm_id[i], NULL, pharma_com, &p[i]) < 0){
            pthread_mutex_lock(&lock_screen);
            printf("\033[0;31m"); // red color 
            perror("pthread ERROR");
            pthread_mutex_unlock(&lock_screen);
            for(int j=0;j<i;j++){
                pthread_kill(pharm_id[j], 1);
            }
            printf("\033[1;32m"); // Bold green color 
            printf("Simulation terminated due to error!\n");
            return 0;
        }
    }// -----------------------------------------> end creation
    // ------------------------------------------>   Create Vaccination zones
    for(int i=0;i<m;i++){
        q[i].zone_num = i; // vac zone ids from 0 to m-1 (output will show 1 to m)
        //q[i].remaining_vaccines = 0; 
        //q[i].company_id_batch = -1;
        if(pthread_create(&vac_zon_id[i], NULL, vaccination_zone, &q[i]) < 0){
            pthread_mutex_lock(&lock_screen);
            printf("\033[0;31m"); // red color 
            perror("pthread ERROR");
            pthread_mutex_unlock(&lock_screen);
            for(int j=0;j<i;j++){
                pthread_kill(vac_zon_id[j], 1);
            }
            printf("\033[1;32m"); // Bold green color 
            printf("Simulation terminated due to error!\n");
            return 0;
        }
    }// -----------------------------------------> end creation
    // ------------------------------------------>   Create Students
    for(int i=0;i<o;i++){
        r[i].stud_num = i; // student ids from 0 to o-1 (output will show 1 to o)
        if(pthread_create(&stud_id[i], NULL, student, &r[i]) < 0){
            pthread_mutex_lock(&lock_screen);
            printf("\033[0;31m"); // red color 
            perror("pthread ERROR");
            pthread_mutex_unlock(&lock_screen);
            for(int j=0;j<i;j++){
                pthread_kill(vac_zon_id[j], 1);
            }
            printf("\033[1;32m"); // Bold green color 
            printf("Simulation terminated due to error!\n");
            return 0;
        }
    }// -----------------------------------------> end creation

    for(int i=0;i<o;i++){
        pthread_join(stud_id[i], NULL);
    }
    terminate = 1;
    for(int i=0;i<m;i++){
        pthread_join(vac_zon_id[i], NULL);
    }
    for(int i=0;i<n;i++){
        pthread_join(pharm_id[i], NULL);
    } 
    printf("\033[1;32m"); // Bold green color 
    printf("Simulation Over!\n");
    printf("\033[0m"); // back to normal black color
    free(batches_ready);
    free(batches_runnin);
    free(vac_per_batch);
    return 0;
}

int initiate_prog(int n, int m, int o){ //------------------------------------------------> Initializing the program
    terminate = 0;
    int max_threads = 1000;
    printf("\033[0;31m"); // All errors in red
    if(n==0){
        printf("Since there is no pharma company to provide vaccines, Rubrik failed to convinve IIITH administration\n");
        return -1;
    }
    if(m==0){
        printf("Since no vaccination zones can be setup(m=0), IIITH rejected Rubrik's idea\n");
        return -1;
    }
    if(o==0){
        printf("No student is willing to come, therefore Rubrik discarded his idea of having an offline semester\n");
        return -1;
    }
    if(n+m+o > max_threads){
        printf("The input crossed the max thread limit, not allowed to simulate\n");
        return -1;
    }
    // locks init
    if (pthread_mutex_init(&lock_pharma_com, NULL) != 0) { // initializing the lock
        perror("Mutex init has failed");
        return -1; 
    }
    if (pthread_mutex_init(&lock_vac_zon, NULL) != 0) { // initializing the lock
        perror("Mutex init has failed"); 
        return -1; 
    }
    if (pthread_mutex_init(&lock_stud, NULL) != 0) { // initializing the lock
        perror("Mutex init has failed"); 
        return -1; 
    } 
    if (pthread_mutex_init(&lock_screen, NULL) != 0) { // initializing the lock
        perror("Mutex init has failed"); 
        return -1; 
    }
    if (pthread_mutex_init(&lock_rand, NULL) != 0) { // initializing the lock
        perror("Mutex init has failed"); 
        return -1; 
    }
    if (pthread_mutex_init(&lock_wait_stud, NULL) != 0) { // initializing the lock
        perror("Mutex init has failed"); 
        return -1; 
    }
    if (pthread_mutex_init(&lock_slot_update, NULL) != 0) { // initializing the lock
        perror("Mutex init has failed"); 
        return -1; 
    }
    /*if (pthread_mutex_init(&lock_batch_update, NULL) != 0) { // initializing the lock
        perror("Mutex init has failed"); 
        return -1; 
    }*/

    // allocating memory to global variables
    if((batches_ready = (int*) malloc(sizeof(int)*(n+2)))==NULL){
        perror("malloc error");
    }
    if((batches_runnin = (int*) malloc(sizeof(int)*(n+2)))==NULL){
        perror("malloc error");
    }
    if((vac_per_batch = (int*) malloc(sizeof(int)*(n+2)))==NULL){
        perror("malloc error");
    }
    if((succ_proba_pharma = (float*) malloc(sizeof(float)*(n+2)))==NULL){
        perror("malloc error");
    }
    if((succ_proba_vaczon = (float*) malloc(sizeof(float)*(m+2)))==NULL){
        perror("malloc error");
    }
    if((slots_available = (int*) malloc(sizeof(int)*(m+2)))==NULL){
        perror("malloc error");
    }
    if((slots_filled = (int*) malloc(sizeof(int)*(m+2)))==NULL){
        perror("malloc error");
    }
    if((flags_between_studs_vaczones = (short int*) malloc(sizeof(short int)*(m+2)))==NULL){
        perror("malloc error");
    }
    for(int i=0;i<n+2;i++) batches_ready[i]=batches_runnin[i]=vac_per_batch[i]=0;
    for(int i=0;i<m+2;i++) slots_available[i] = flags_between_studs_vaczones[i] = slots_filled[i] = 0;
    no_of_studs_waiting = 0;
    //batches_ready_for_distribution = 0;
    printf("\033[0m"); // back to normal black color
}

void* pharma_com(void* a){// -------------------------------------------------------------> Pharma Companies
    pharma_thread_struct *args = (pharma_thread_struct*) a;
    int id = args->pharma_com_num;
    while(1){
        if(terminate==1)
            break;

        while((batches_runnin[id]>0 || batches_ready[id] >0)&& terminate==0){
        }
        //printf("Batches runnin:%d\nBatches ready:%d\nterminat=%d\n", batches_runnin[id], batches_ready[id], terminate);
        if(terminate==1){
            break;
        }
        pthread_mutex_lock(&lock_rand);
        int a = rand()%5 + 1;
        int b = rand()%11 + 10; // since no of vaccines will be between 10 and 20
        int x = rand()%4 + 2; // sleep time (prep time)
        pthread_mutex_unlock(&lock_rand);
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;34m");
        printf("Pharmaceutical Company %d is preparing %d batches of vaccine which have success probability %0.3f\n", id+1, a, succ_proba_pharma[id]);
        pthread_mutex_unlock(&lock_screen);
        sleep(x); // preparation (or sleeping here)
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;34m");
        printf("Pharmaceutical Company %d has prepared %d batches of vaccine which have success probability %0.3f. Waiting for all the vaccines to be used to resume production\n", id+1, a, succ_proba_pharma[id]);
        pthread_mutex_unlock(&lock_screen);
        batches_ready[id] = a; // batches from 1 to 5
        vac_per_batch[id] = b;
        //pthread_mutex_lock(&lock_batch_update);
        //batches_ready_for_distribution += batches_ready[id];
        //pthread_mutex_unlock(&lock_batch_update);
    }
}

void* vaccination_zone(void* a){ // -------------------------------------------------------->Vaccination Zones
    // We need to first wait until we get our first batch of vaccines
    vaczone_thread_struct *args = (vaczone_thread_struct*) a;
    int id = args->zone_num;
    int remaining_vaccines = 0;
    int company_id_batch = -1;
    while(terminate==0)
    {
        if(terminate==1) break;
        int batch_ass = -1;

        while(remaining_vaccines==0 && terminate==0){
            for(int i=0;i<n;i++){
                if(batches_ready[i] > 0){
                    pthread_mutex_lock(&lock_vac_zon);
                    if(batches_ready[i]==0){
                        pthread_mutex_unlock(&lock_vac_zon);
                        continue;
                    }
                    company_id_batch = i; //(i+1 actual id)
                    remaining_vaccines = vac_per_batch[i];
                    batches_runnin[i]++;
                    batches_ready[company_id_batch]--; // synchronization
                    pthread_mutex_unlock(&lock_vac_zon);
                    break;
                }
                if(i==n-1)
                    i = -1;// start from i=0 again
            }
            if(remaining_vaccines!=0){
                pthread_mutex_lock(&lock_screen);
                printf("\033[0;34m");
                printf("Pharmaceutical Company %d is delivering a vaccine batch to Vaccination Zone %d which has success probability %0.3f\n", company_id_batch+1, id+1, succ_proba_pharma[company_id_batch]);
                printf("\033[0;34m");
                printf("Pharmaceutical company %d delivered a vaccine batch to Vaccination zone %d, resuming vaccinations now\n", company_id_batch+1, id+1);
                printf("No of vaccines delivered in total=%d\n", remaining_vaccines);
                pthread_mutex_unlock(&lock_screen);
            }
        }

        succ_proba_vaczon[id] = succ_proba_pharma[company_id_batch];

        if(terminate==1) break;

        flags_between_studs_vaczones[id] = 0;
        int no_of_slots = 1;
        if(remaining_vaccines>1 && no_of_studs_waiting>1){
            pthread_mutex_lock(&lock_rand);
            int e = rand();
            pthread_mutex_unlock(&lock_rand);
            no_of_slots = e%(minimum(8, no_of_studs_waiting, remaining_vaccines)) + 1;
        }
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;33m");
        printf("Vaccination Zone %d is ready to vaccinate with %d slots, waiting list=%d \n", id+1, no_of_slots, no_of_studs_waiting);
        pthread_mutex_unlock(&lock_screen);
        slots_available[id] = no_of_slots;
        
        while((slots_filled[id]<=0 && no_of_studs_waiting!=0) && terminate==0){
        }
        while(slots_filled[id]!=no_of_slots && no_of_studs_waiting!=0  && terminate==0){
        }
        remaining_vaccines = remaining_vaccines - slots_filled[id];
        slots_available[id] = 0;
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;33m");
        /*printf("Slots filled:%d\n", slots_filled[id]);
        printf("Slots available:%d\n", slots_available[id]);*/
        printf("Vaccination Zone %d entering Vaccination Phase with %d students\n", id+1, slots_filled[id]);
        pthread_mutex_unlock(&lock_screen);
        flags_between_studs_vaczones[id] = 1; // start vacccination
        while(slots_filled[id]>0);
        /*pthread_mutex_lock(&lock_screen);
        printf("\033[0;33m");
        printf("Vaccination Zone %d slots filled=%d\n", id+1, slots_filled[id]);
        pthread_mutex_unlock(&lock_screen);*/
        flags_between_studs_vaczones[id] = 0; // end vaccination
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;33m");
        printf("Vaccination Zone %d vaccination phase completed\n", id+1);
        printf("batches runnin %d\n", batches_runnin[company_id_batch]);
        printf("batches ready %d\n", batches_ready[company_id_batch]);
        pthread_mutex_unlock(&lock_screen);
        if(remaining_vaccines==0){
            pthread_mutex_lock(&lock_screen);
            printf("\033[0;33m");
            printf("Vaccination Zone %d has run out of vaccines\n", id+1);
            pthread_mutex_unlock(&lock_screen);
        }
    }
}

void* student(void* a){
    // Since a student can arrive at a random time
    // Therefore, we assume that he comes in the interval [1, o/2] secs, where o = #students
    
    int id;
    if(o>2){
        pthread_mutex_lock(&lock_rand);
        id  = rand()%(o/2) + 1;
        pthread_mutex_unlock(&lock_rand);
        sleep(id);
    }
    stud_thread_struct *args = (stud_thread_struct*) a;
    id = args->stud_num;
    short int flag = -1;
    for(int i=0;i<3;i++){
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;36m"); // cyan
        printf("Student %d has arrived for his round number %d of Vaccination\n", id+1, i+1);
        pthread_mutex_unlock(&lock_screen);
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;36m"); // cyan
        printf("Student %d is waiting to be allocated a slot on a Vaccination Zone\n", id+1);
        pthread_mutex_unlock(&lock_screen);

        pthread_mutex_lock(&lock_wait_stud);
        no_of_studs_waiting++;
        pthread_mutex_unlock(&lock_wait_stud);
        /*if(i==2){
                pthread_mutex_lock(&lock_screen);
                printf("\033[0;36m"); // cyan
                printf("Student %d and Students waiting=%d\n", id+1, no_of_studs_waiting);
                pthread_mutex_unlock(&lock_screen);
            }*/
        int vaczon_id = -1;
        for(int j=0;j<m;j++){
            if(slots_available[j] > 0  && flags_between_studs_vaczones[j]==0){
                pthread_mutex_lock(&lock_stud);
                if(slots_available[j] > 0  && flags_between_studs_vaczones[j]==0){
                    vaczon_id = j;
                    pthread_mutex_lock(&lock_screen);
                    printf("\033[0;36m");
                    printf("Student %d assigned a slot on the Vaccination Zone %d and waiting to be vaccinated\n", id+1, vaczon_id+1);
                    pthread_mutex_unlock(&lock_screen);
                    slots_available[j]--;
                    slots_filled[j]++;
                }
                else{
                    pthread_mutex_unlock(&lock_stud);
                    continue;
                }
                pthread_mutex_unlock(&lock_stud);
                pthread_mutex_lock(&lock_wait_stud);
                no_of_studs_waiting--;
                pthread_mutex_unlock(&lock_wait_stud);
            }
            if(vaczon_id>=0) break;
            if(j==m-1) j = -1; // restart searching
        }
        while(flags_between_studs_vaczones[vaczon_id] ==0);
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;36m");
        printf("Student %d on Vaccination Zone %d has been vaccinated which has success probability %0.3f\n", id+1, vaczon_id+1, succ_proba_vaczon[vaczon_id]);
        pthread_mutex_unlock(&lock_screen);
        // Anti body test
        pthread_mutex_lock(&lock_rand);
        float x = rand()%1001;
        pthread_mutex_unlock(&lock_rand);
        x = x/1000;
        if(x <= succ_proba_vaczon[vaczon_id]){ // test success
            pthread_mutex_lock(&lock_screen);
            printf("\033[0;36m");
            printf("Student %d has tested POSITIVE for antibodies and finally goes to college\n", id+1);
            pthread_mutex_unlock(&lock_screen);
            pthread_mutex_lock(&lock_slot_update);
            slots_filled[vaczon_id]--;
            pthread_mutex_unlock(&lock_slot_update);
            flag = 1;
            break;
        }
        else{ // test failed
            pthread_mutex_lock(&lock_screen);
            printf("\033[0;36m");
            printf("Student %d has tested NEGITIVE for antibodies.\n", id+1);
            pthread_mutex_unlock(&lock_screen);
            pthread_mutex_lock(&lock_slot_update);
            slots_filled[vaczon_id]--;
            pthread_mutex_unlock(&lock_slot_update);
            if(i==2)
                break;
        }
    }
    if(flag==-1){
        pthread_mutex_lock(&lock_screen);
        printf("\033[0;36m");
        printf("Student %d is sent back to home due failed vaccination\n", id+1);
        pthread_mutex_unlock(&lock_screen);
    }
}

int minimum(int a, int b, int c){
    if(a<=b && a<=c) return a;
    if(b<=a && b<=c) return b;
    if(c<=a && c<=b) return c;
}
