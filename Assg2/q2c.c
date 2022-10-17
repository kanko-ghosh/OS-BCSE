#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define NO_OF_PROCESS 3
#define NO_OF_ITERATION 10

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

int main(int argc, char const *argv[])
{

    int parent_pid = getpid();
    pid_t *child = (pid_t *)malloc(NO_OF_PROCESS * sizeof(pid_t));

    sem_t **sem = (sem_t **)malloc((NO_OF_PROCESS) * sizeof(sem_t *));
    for (int i = 0; i < NO_OF_PROCESS; i++){
        char * itcnt = uitoa(i);            
        sem_unlink(itcnt);
        sem[i] = sem_open(itcnt, O_CREAT, 0660, 1);
        free(itcnt);
    }

    sem_unlink("mutex");
    sem_unlink("mutex2");
    sem_t *mutex = sem_open("mutex", O_CREAT, 0660, 1);
    sem_t *mutex2 = sem_open("mutex2", O_CREAT, 0660, 1);
    
    sem_unlink("count");
    sem_t *count = sem_open("count", O_CREAT, 0660, NO_OF_PROCESS);

    for (int i = 0; i < NO_OF_PROCESS; i++){
        child[i] = fork();
        if (child[i] < 0) {
            //error
            printf("Forking failed\n");
            printf("Child no: %d", i);
            return 1;
        } else if (child[i] == 0){
            /**
             * @brief 
             * child -> 
             * do the loop here
             * 10 iterations
             */
            pid_t temp = getpid();
            srand(temp);
            for (int j = 0; j < NO_OF_ITERATION; j++){
                // sem_wait(mutex2);
                sem_wait(sem[i]);
                sem_wait(count);                
                printf("Process PID: %d\tIteration count: %d\n", i, j);
                sleep(sleep(rand()%10));
                sem_wait(mutex);
                int val;
                int t = sem_getvalue(count, &val);
                if (!val){
                    for (int k = 0; k < NO_OF_PROCESS; k++){
                        sem_post(sem[k]);
                        sem_post(count);
                    }
                }
                sem_post(mutex);
                // sem_post(mutex2);
            }
            break;


        } else {
            /**
             * @brief 
             * parent ->
             * print details
             */
            printf("Child process: %d, pid: %d \n", i, child[i]); 
        }
    }


    if (getpid() == parent_pid){
        int v;
        for (int i = 0; i < NO_OF_PROCESS; i++){
            waitpid(child[i], &v, 0);
        }
        free(child);
        for (int j = 0; j < NO_OF_PROCESS; j++){
            char * itcnt = uitoa(j);
            sem_unlink(itcnt);
            free(itcnt);
        }
        free(sem);
    }
    return 0;
}
