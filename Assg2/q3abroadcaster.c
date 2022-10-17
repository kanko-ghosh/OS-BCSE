#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define NO_OF_RECEIVER 2

int main(){
    int fd;
    char * fifo_path "/temp/fifo";
    mkfifo(fifo_path, 0666);

    int n_of_r;
    printf("Number of receivers: ");
    scanf("%d", &n_of_r);

    sem_t * sem[n_of_r];
    for (int i = 0; i < n_of_r; i++){
        sem_unlink(itoa(i));
        sem[i] = sem_open(itoa(i), O_CREAT, 0660, 0);
    }

    sem_t *sem_count = sem_open("count", O_CREAT, 0660, 0);
    sem_t *sem_check = sem_open("check", O_CREAT, 0660, 0);

    char data[50];
    printf("Enter Data to broadcast: ");
    scanf("%s", data);

    while (strcmp(data, "$")){

    }

    printf("End\n");

    sem_unlink("count");
    sem_unlink("check");
    for (int i = 0; i < n_of_r; i++){
        sem_unlink(itoa(i));
    }
    return 0;
}