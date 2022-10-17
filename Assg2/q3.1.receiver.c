#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

char * uitoa(int num){
    char * str;
    str = (char *)malloc(5 * sizeof(char));
    str[4] = 0;
    for (int i = 3; i >= 0; i--){
        str[i] = (num%10)+'0';
        num/=10;
    }
    return str;
}


int main(){
    int fifo_file;
    char *fifo_name = "/tmp/myFifo";
    mkfifo(fifo_name, 0666);
    char buffer[80] = "";
    int r_cnt;
    sem_t *notify_on_sent = sem_open("notify_on_sent", O_CREAT, 0660, 0);
    sem_t *get_value = sem_open("get_value", O_CREAT, 0660, 0);
    char * temp = strcat(uitoa(r_cnt), "sent");
    sem_t *received = sem_open(temp, O_CREAT, 0660, 0);

    printf("Enter receiver number: ");
    scanf("%d", &r_cnt);
        printf("lol\n");

    while(strcmp(buffer, "!end")) {
        sem_wait(received);
        fifo_file = open(fifo_name, O_RDONLY);  // read only mode
        read(fifo_file, buffer, 80);
        printf("Message received: %s\n", buffer);
        close(fifo_file);
        sem_post(notify_on_sent);
        sem_post(get_value);
    }

}