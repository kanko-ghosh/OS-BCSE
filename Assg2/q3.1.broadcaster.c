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

    printf("Enter number of receivers: ");
    scanf("%d", &r_cnt);

    sem_unlink("notify_on_sent");
    sem_unlink("get_value");
    sem_t *notify_on_sent = sem_open("notify_on_sent", O_CREAT, 0660, 0);
    sem_t *get_value = sem_open("get_value", O_CREAT, 0660, 0);

    sem_t *sent[r_cnt];
    for (int i = 0; i < r_cnt; i++){
        char * temp = strcat(uitoa(i), "sent");
        sem_unlink(temp);
        sent[i] = sem_open(temp,  O_CREAT, 0660, 0);
    }


    while(strcmp(buffer, "!end")){
        printf("Enter message: \n");
        scanf("%s", buffer);
            printf("lol\n");
        fifo_file = open(fifo_name, O_WRONLY);
        for (int i = 0; i < r_cnt; i++){
            write(fifo_file, buffer, 80);
            sem_post(sent[i]);
        }
        close(fifo_file);

        // for (int i = 0; i < r_cnt; i++){
        // }

        while (1){
            sem_wait(notify_on_sent);
            int val;
            sem_getvalue(get_value, &val);
            if (val == r_cnt){
                while (val--){
                    sem_wait(get_value);
                }
                break;
            }
        }
    }



}