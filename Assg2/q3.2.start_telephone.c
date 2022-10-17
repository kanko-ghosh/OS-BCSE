#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
    int fifo_file;
    char *fifo_name = "/tmp/myFifo";
    mkfifo(fifo_name, 0666);
    char buffer[80] = "";

    sem_unlink("caller");
    sem_unlink("receiver");

    sem_t *caller = sem_open("caller", O_CREAT, 0660, 0);
    sem_t *receiver = sem_open("receiver", O_CREAT, 0660, 0);

    if (caller == SEM_FAILED || receiver == SEM_FAILED){
        printf("lol");
        return 1;
    }

    printf("Call received! start chatting!\n");

    while (strcmp(buffer, "$")) {
        sem_wait(caller);
            fifo_file = open(fifo_name, O_WRONLY);
            printf("Enter Text: ");
            scanf("%[^\n]s", buffer);
            getchar();

            write(fifo_file, buffer, 80);
            close(fifo_file);

            if (!strcmp(buffer, "$")) {
                break;
            }
        sem_post(receiver);
        fifo_file = open(fifo_name, O_RDONLY);

        read(fifo_file, buffer, 80);
        printf("Message Received: %s\n", buffer);
        close(fifo_file);
    }

    printf("Call Ended!\n");

    sem_unlink("caller");
    sem_unlink("receiver");

    return 0;
}