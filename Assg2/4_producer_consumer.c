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

typedef char bool;
bool true = 1;
bool false = 0;

#define MAX_BUFFER_SIZE 1024


typedef struct __Data
{
    int producerId;
    int data;
} Data;

// Data INVALID_DATA;
// INVALID_DATA.producerId = INVALID_DATA.data = -1;

typedef struct __CircularBuffer
{
    // Data *buffer;
    Data buffer[MAX_BUFFER_SIZE];
    int size, maxSize;
    int front, back;
} CircularBuffer;

void createCircularBuffer(CircularBuffer *cb, int size)
{
    cb->maxSize = size;
    // cb->buffer = (Data *)malloc(cb->maxSize * sizeof(Data));
    cb->size = cb->front = cb->back = 0;
}

void push(CircularBuffer *cb, Data d)
{
    if(cb->size >= cb->maxSize) return;

    cb->buffer[(cb->front++) % cb->maxSize] = d;
    cb->size++;
}

Data pop(CircularBuffer *cb)
{
    Data INVALID_DATA;
    INVALID_DATA.producerId = INVALID_DATA.data = -1;
    if(cb->size <= 0) return INVALID_DATA;

    cb->size--;
    return cb->buffer[(cb->back++) % cb->maxSize];
}


int main()
{
    int p, c, n;
    // Need shared memory for total
    int *TOTAL = (int *)mmap(NULL , sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    *TOTAL = 0;

    printf("Enter #producers: ");
    scanf("%d", &p);
    printf("Enter #consumers: ");
    scanf("%d", &c);
    printf("Enter size of circular buffer: ");
    scanf("%d", &n);

    // Semaphores ...
    sem_unlink("mutex"), sem_unlink("empty"), sem_unlink("full"), sem_unlink("prodCntLeft");
    sem_t *mutex, *empty, *full, *prodCntLeft;
    if((mutex = sem_open("mutex", O_CREAT, 0660, 1)) == SEM_FAILED){ printf("Sem failed mutex !!!\n"); return 1; }
    if((empty = sem_open("empty", O_CREAT, 0660, n)) == SEM_FAILED){ printf("Sem failed empty !!!\n"); return 1; }
    if((full = sem_open("full", O_CREAT, 0660, 0)) == SEM_FAILED){ printf("Sem failed full !!!\n"); return 1; }
    if((prodCntLeft = sem_open("prodCntLeft", O_CREAT, 0660, p)) == SEM_FAILED){ printf("Sem failed prodCntLeft !!!\n"); return 1; }

    // Need shared memory for circular buffer
    CircularBuffer *buffer = (CircularBuffer *)mmap(NULL , sizeof(CircularBuffer), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    createCircularBuffer(buffer, n);
    pid_t producer[p], consumer[c];

    for(int i = 0; i < p; i++)
    {
        producer[i] = fork();

        if(producer[i] < 0)
        {
            printf("Fork producer #%d failed, exiting ...\n", i);
            return 1;
        }
        else if(producer[i] == 0)
        {
            // Producer Subprocess
            bool allProducerDone = false;
            int val;
            Data d;

            sem_wait(empty);
            sem_wait(mutex);

            d.data = d.producerId = i;
            push(buffer, d);
            printf("Producer #%d produces data = %d  [BUFFER SIZE = %d] ...\n", i, i, buffer->size);
            sem_wait(prodCntLeft);
            sem_getvalue(prodCntLeft, &val);
            if(!val) allProducerDone = true;

            sem_post(mutex);
            sem_post(full);

            if(allProducerDone)
            {
                printf("\t\t___ ALL PRODUCERS DONE !!! ___\n");
                for(int j = 0; j < c; j++) sem_post(full);  // When consumers see buffer is empty, they will know all producers are dead and exit ...
            }
            // sem_post_multiple(full, c);  // doesnot work

            return 0;
        }
        else
        {
            // returns pid of child to parent process
            printf("\t\t---------- Producer Process #%d created (pid: %d) ----------\n", i, producer[i]);
        }
    }

    for(int i = 0; i < c; i++)
    {
        consumer[i] = fork();

        if(consumer[i] < 0)
        {
            printf("Fork consumer #%d failed, exiting ...\n", i);
            return 1;
        }
        else if(consumer[i] == 0)
        {
            // Consumer Subprocess
            while(true)
            {
                // Every consumer keeps on consuming till producers can produce ...
                Data d;

                sem_wait(full);
                sem_wait(mutex);

                // printf("Debug consumer --> %d\n", buffer->size);
                if(buffer->size <= 0)
                {
                    printf("\t--- Buffer EMPTY , consumer #%d exitting ---\n", i);
                    sem_post(mutex);
                    return 0;
                }
                d = pop(buffer);
                printf("Consumer #%d gets data = %d from producer #%d\n", i, d.data, d.producerId);
                *TOTAL = *TOTAL + d.data;

                sem_post(mutex);
                sem_post(empty);
            }
        }
        else
        {
            // returns pid of child to parent process
            printf("\t\t---------- Consumer Process #%d created (pid: %d) ----------\n", i, consumer[i]);
        }
    }

    int stat;
    // printf("lol\n");
    for(int i = 0; i < c; i++) waitpid(consumer[i], &stat, 0);  // printf("lol2\n");
    for(int i = 0; i < p; i++) waitpid(producer[i], &stat, 0);  // printf("lol1\n");


    sem_unlink("mutex"), sem_unlink("empty"), sem_unlink("full"), sem_unlink("prodCntLeft");
    sem_destroy(mutex), sem_destroy(empty), sem_destroy(full), sem_destroy(prodCntLeft);

    printf("\n\nFinal Total Value = %d\n", *TOTAL);

    return 0;
}
