#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
// #include <semaphore.h>
// #include <fcntl.h>

#define SLEEP 0.1
#define NO_OF_PROCESS 3
int main(int argc, char const *argv[])
{

    int parent_pid = getpid();
    //2 processes

    pid_t *child = (pid_t *)malloc(NO_OF_PROCESS * sizeof(pid_t));

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
            srand(getpid());
            for (int j = 0; j < 10; j++){
                // sleep(((float)rand() / (RAND_MAX)) * SLEEP);
                printf("Process PID: %d\tParent PID: %d\tIteration count: %d\n", getpid(), getppid(), j);
                sleep(rand()%10);
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
        for (int i = 0; i < 2; i++){
            waitpid(child[i], &v, 0);
        }
        free(child);
    }
    return 0;
}
