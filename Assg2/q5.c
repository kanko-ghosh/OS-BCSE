// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #include <unistd.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <semaphore.h>
// #include <fcntl.h>

// int main(){
//     FILE *fp;
//     char * filename = "r_w.txt";

//     int *r = (int *)malloc(sizeof(int));
//     int *w = (int *)malloc(sizeof(int));
//     printf("Enter Number of readers: ");
//     scanf("%d", r);
//     printf("Enter Number of writers: ");
//     scanf("%d", w);

//     int * readcount = (int)malloc(sizeof(int));
//     *readcount = 0;

//     sem_t *mutex = sem_open("mutex", O_CREAT, 0660, 1);
//     sem_t *wrt = sem_open("wrt", O_CREAT, 0660, 1);

//     for (int i = 0; i < r+w;){
//         pid_t child = fork();
//         if (child < 0){
//             printf("Forking failed\n");
//             printf("Child no: %d", i);
//             return 1;
//         } else if (chid == 0){
//             srand(getpid());
//             sleep(rand()%10);
//             if (getpid()%2 == 0){
//                 //reader
//                 r--;
//                 sem_wait(mutex);
//                 (*readcount)++;
//                 if(*readcount==1){
//                     sem_wait(wrt);
//                 }
//                 sem_post(mutex);


//                 fp = fopen(filename, "r");
//                 char str[80];
//                 fgets(str, 80, fp);
//                 printf("Reader Number: %d -> output: %s", , str);
//                 fclose(fp);

//                 sem_wait(mutex);
//                 (*readcount)--;
//                 if (*readcount==0){
//                     sem_post(wrt);
//                 }
//                 sem_post(mutex);
//             } else {
//                 //writer
//                 wait(wrt);

//                 fp = fopen(filename, "a");
//                 fprintf(fp, "[d] ");
//                 fclose(fp);

//                 signal(wrt);
//             }
//         }
//     }

// }


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>


typedef struct{
    int item_id;
    int producer_id;
    int payload;
}buffer;

typedef struct{
    sem_t mut;
    int i;
}semint;

auto producerFunc = [&](int a){
    while(true){
        sem_wait(&full->mut);
        sem_wait(&mutexval->mut);

        buffer newprod;
        newprod.producer_id = a;
        newprod.payload = rand()%10000;

        produced[i] = newprod;
        i++;

        cout << "Producer : " << newprod.producer_id << ' ' << newprod.payload << " " << "\n" ;

        sem_post(&mutexval->mut);
        sem_post(&empty->mut);
    }

};

int main(){
    semint *mutexval;
    semint *empty;
    semint *full;

    mutexval = (semint *)malloc(sizeof(semint));
    empty = (semint *)malloc(sizeof(semint));
    full = (semint *)malloc(sizeof(semint));
    int buffersize;
    printf("Enter buffer size: ");
    scanf("%d", &buffersize);

    buffer produced[buffersize];

    sem_init(&mutexval->mut, 0, 1);
    sem_init(&empty->mut, 0, 0);
    sem_init(&full->mut, 0, buffersize);

    int producers;
    int consumers;

    printf("Enter the number of producers: ")
    scanf("%d", &producers);
    printf("Enter the number of consumers: ")
    scanf("%d", &consumers);
    int i = 0;



    auto consumerFunc = [&](int a){
        while(true){
            sem_wait(&empty->mut);
            sem_wait(&mutexval->mut);

            buffer newprod;  
            i--;
            newprod = produced[i];
            cout << "Consumer : " << newprod.producer_id << ' ' << newprod.payload << " " << a << "\n" ;

            sem_post(&mutexval->mut);
            sem_post(&full->mut);
            sleep(3);
        }
    };

    vector<thread> threads;

    for(int i = 0 ; i < producers ; i++){
        thread t(producerFunc, i);
        threads.push_back(move(t));
    }

    for(int j = 0 ; j < consumers ; j++){
        thread t(consumerFunc, j);
        threads.push_back(move(t));
    }

    for(auto &t : threads){
        t.join();
    }


}