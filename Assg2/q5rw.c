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
    int r_cnt = 10;
    int w_cnt = 10;
    int read_iteration_count = 20;

    int *buffer = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    int *reader_cnt = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    int *writer_cnt = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);

    *buffer =  -1; //init
    *reader_cnt = 0;
    *writer_cnt = 0;

    pid_t readers[r_cnt];
    pid_t writers[w_cnt];

    int t;

    //semaphores
    sem_unlink("wrt");
    sem_unlink("mutex");
    sem_unlink("w_priority");
    sem_unlink("start");

    sem_t *wrt = sem_open("wrt", O_CREAT, 0660, 1);
    sem_t *mutex = sem_open("mutex", O_CREAT, 0660, 1);
    sem_t *w_priority = sem_open("w_priority", O_CREAT, 0660, 1);
    sem_t *start = sem_open("start", O_CREAT, 0660, 0);
    sem_t *is_writer_done[r_cnt];
    for (int i = 0; i < r_cnt; i++){
        char * temp = uitoa(i);
        char * temp2 = strcat(temp, "is_writer_done");
        sem_unlink(temp2);
        is_writer_done[i] = sem_open(temp2, O_CREAT, 0660, 0);
    }


    


    //reader processes
    for (int i = 0; i < r_cnt; i++){
        readers[i] = fork();
        if (readers[i] < 0){
            perror("Forking failed\n");
            return 1;
        } else if (readers[i] == 0) {
            srand(getpid());
            sleep(sleep(rand()%10));   
                sem_wait(start);
                
                //read and print this many times!!
                while (*writer_cnt > 0)
                    sem_wait(is_writer_done[i]);
                    //while since is_writer_done maybe > 0

                sem_wait(mutex);
                  *reader_cnt = *reader_cnt + 1;
                  if(*reader_cnt == 1) sem_wait(wrt);
                sem_post(mutex);

                printf("Reader cnt: %d\tValue: %d \n", i, *buffer);

                sem_wait(mutex);
                  *reader_cnt = *reader_cnt - 1;
                  if(*reader_cnt == 0) sem_post(wrt);
                sem_post(mutex);
            return 0;
        } else {
            printf("Reader No: %d created\n", i);
        }
    }

    //writer processes
    for (int i = 0; i < w_cnt; i++){
        writers[i] = fork();
        if (writers[i] < 0){
            perror("Forking failed\n");
            return 1;
        } else if (writers[i] == 0) {
            srand(getpid());
            sleep(sleep(rand()%10));    

            sem_wait(start);
            
            sem_wait(w_priority);
            *writer_cnt = *writer_cnt + 1;
            sem_post(w_priority);

            sem_wait(wrt);
            *buffer = i;
            printf("Writer #%d wrote data = %d\n", i, i);
            sem_post(wrt);

            sem_wait(w_priority);
            *writer_cnt = *writer_cnt - 1;
            sem_post(w_priority);

            for(int j = 0; j < r_cnt; j++) sem_post(is_writer_done[j]);

            return 0;
        } else {
            printf("Writer No: %d created\n", i);
        }
    }

    int v;
    for (int i = 0; i < r_cnt+w_cnt; i++){
        sem_post(start);
    }
    for (int i = 0; i < r_cnt; i++){
        waitpid(readers[i], &v, 0);
    }
    for (int i = 0; i < w_cnt; i++){
        waitpid(writers[i], &v, 0);
    }

    sem_unlink("wrt");
    sem_unlink("mutex");
    sem_unlink("start");
    sem_unlink("w_priority");
    for (int i = 0; i < r_cnt; i++){
        char * temp = uitoa(i);
        char * temp2 = strcat(temp, "is_writer_done");
        sem_unlink(temp2);
    }
    return 0;
}