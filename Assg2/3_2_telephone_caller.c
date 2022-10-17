#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>


#define BUFFER_SIZE 1024

typedef char bool;
bool true = 1;
bool false = 0;


int main()
{
    int fd;
    char *myFifo = "/tmp/myFifo";
    mkfifo(myFifo, 0666);
    char buffer[BUFFER_SIZE];
    bool callEndedBySelf = false;

    // Semaphores ...
    sem_unlink("cs"), sem_unlink("rs");
    sem_t *callerCanSend, *receiverCanSend;
    if((callerCanSend = sem_open("cs", O_CREAT, 0660, 0)) == SEM_FAILED){ printf("Sem failed callerCanSend !!!\n"); return 1; }
    if((receiverCanSend = sem_open("rs", O_CREAT, 0660, 0)) == SEM_FAILED){ printf("Sem failed callerCanSend !!!\n"); return 1; }


    printf("You are the CALLER !!!\n");
    printf("Enter \"!end\" to end (without quotes) ...\n\n");

    do
    {
        // SENDING ...
        fd = open(myFifo, O_WRONLY);  // write only mode

        sem_wait(callerCanSend);
        // printf("lol\n");
        printf("Enter your message: ");
        scanf("%[^\n]s", buffer);
        getchar();  // to remove dummy newline

        write(fd, buffer, strlen(buffer) + 1);  // +1 to account for '\n'
        close(fd);

        if(!strcmp(buffer, "!end")){ callEndedBySelf = true; break; }


        // RECEIVING ...
        fd = open(myFifo, O_RDONLY);  // read only mode
        sem_post(receiverCanSend);
        read(fd, buffer, BUFFER_SIZE);
        printf("Message received: %s\n", buffer);
        close(fd);
    }
    while(strcmp(buffer, "!end"));

    printf("\t--- CALL ENDED BY %s ---\n", (callEndedBySelf)? "CALLER (You)": "RECEIVER");
    printf("Program ended ...\n");

    sem_destroy(receiverCanSend), sem_destroy(callerCanSend);
    sem_unlink("cs"), sem_unlink("rs");

    return 0;
}
