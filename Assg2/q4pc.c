#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_LENGTH 1000

typedef struct Data {
    int data;
    int pid;
} Data;

typedef struct Bounded_Buffer {
    Data buffer[BUFFER_LENGTH];
    int cur_size;
    int max_size;
    int front; 
    int back;
} Bounded_Buffer;

void init (Bounded_Buffer *b, int size){
    b->cur_size = 0;
    b->front = 0;
    b->back = 0;
    b->max_size = size;
}

int push (Bounded_Buffer *b, Data data){
    
    if(b->cur_size >= b->max_size) 
        return -1;
    b->buffer[b->front % b->max_size] = data;
    b->cur_size++;
    b->front++;
    return 0;
}

Data pop(Bounded_Buffer *b){

    if(b->cur_size <= 0) {
        Data Err;
        Err.pid = -1;
        Err.data = -1;
        return Err;
    }
    b->cur_size--;
    b->back++;
    return b->buffer[(b->back-1) % b->max_size];

}

int main () {

    int p_cnt = 100;
    int c_cnt = 100;

    printf("Enter number of producers: ");
    scanf("%d", &p_cnt);
    printf("Enter number of consumers: ");
    scanf("%d", &c_cnt);

    int buffer_size = 25;

    sem_unlink("mutex");
    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("start");

    sem_t *mutex = sem_open("mutex", O_CREAT, 0660, 1);
    sem_t *empty = sem_open("empty", O_CREAT, 0660, buffer_size);
    sem_t *full = sem_open("full", O_CREAT, 0660, 0);
    sem_t *start = sem_open("start", O_CREAT, 0660, 0);

    Bounded_Buffer *buffer = (Bounded_Buffer *)mmap(NULL , sizeof(Bounded_Buffer), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    init(buffer, buffer_size);
    pid_t ps[p_cnt];
    pid_t cs[c_cnt];

    for (int i = 0; i < p_cnt; i++){
        ps[i] = fork();
        if (ps[i] < 0) {
            perror("Forking failed\n");
            return 1;
        } else if (ps[i] == 0) {
            sem_wait(start);
            sem_wait(empty);
            sem_wait(mutex);
                srand(getpid());
                Data d;
                d.data = rand()%80;
                d.pid = getpid();
                int v = push(buffer, d);
                if (v == -1 ){
                    printf("P_ID: %d\t failed, buffer full", d.pid);
                } else {
                    printf("P_ID: %d\t number of items: %d\n", d.pid, d.data);
                }
            sem_post(mutex);
            sem_post(full);
            return 0;
        } else {
            printf("Producer No: %d created, pid: %d\n", i, ps[i]);
        }
    }

    for (int j = 0; j < c_cnt; j++){
        cs[j] = fork();
        if (cs[j] < 0) {
            perror("Forking failed\n");
            return 1;
        } else if (cs[j] == 0) {
            sem_wait(start);
            sem_wait(full);
            sem_wait(mutex);            

            Data d = pop(buffer);
            if (d.pid == -1){
                printf("C_ID: %d couldnt consume, buffer empty\n", getpid());
            } else {
                printf("C_ID: %d\t consumed data: %d\t from P_ID: %d\n", getpid(), d.data, d.pid);
            }

            sem_post(mutex);
            sem_post(empty);
            return 0;
        } else {
            printf("Consumer No: %d created\n", j);
        }
    }

    int v;
    for (int i = 0; i < p_cnt+c_cnt; i++){
        sem_post(start);
    }
    for (int i = 0; i < p_cnt; i++){
        waitpid(ps[i], &v, 0);
    }
    for (int i = 0; i < c_cnt; i++){
        waitpid(cs[i], &v, 0);
    }

    sem_unlink("full");
    sem_unlink("mutex");
    sem_unlink("empty");
    sem_unlink("start");
    return 0;
}
