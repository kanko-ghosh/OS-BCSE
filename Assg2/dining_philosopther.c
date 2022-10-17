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

#define NOP 7

#define THINKING 0
#define HUNGRY 1
#define EATING 2

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

typedef struct condition {
    sem_t *t;
    int count;
} Condition;

typedef struct DP {
    sem_t * mutex;
    sem_t * next;
    int next_count;
    int state[NOP];
    Condition x[NOP];
    // int turn[NOP];
} DP;

void cond_wait(DP * dp, int i){

    

    dp->x[i].count++;
    if (dp->next_count > 0){
        sem_post(dp->next);
    } else {
        sem_post(dp->mutex);
    }
    sem_wait(dp->x[i].t);
    dp->x[i].count--;
}

void cond_signal (DP * dp, int i){
    if (dp->x[i].count > 0){
		dp->next_count++;
		sem_post(dp->x[i].t);
		sem_wait(dp->next);
		dp->next_count--;
    }
}

void test (DP * dp, int i){
    if (dp->state[(i+NOP-1)%NOP] != EATING && dp->state[(i+1)% NOP] != EATING && dp->state[i] == HUNGRY){ // && dp->turn[i] == i && dp->turn[(i+NOP-1)%NOP] == i){
        dp->state[i] = EATING;
        cond_signal(dp, i);
    }
}

void pickup(DP * dp, int i){

    sem_wait(dp->mutex);
        dp->state[i] = HUNGRY;
        test(dp, i);
        if (dp->state[i] == HUNGRY){
            cond_wait(dp, i);
        }
        printf("%d got the chopsticks\n", i);
    if (dp->next_count > 0){
        sem_post(dp->next);
    } else {
        sem_post(dp->mutex);
    }
}

void putdown(DP * dp, int i){

    sem_wait(dp->mutex);
        dp->state[i] = THINKING;
        // dp->turn[i] = (i+1)%NOP;
	    // dp->turn[(i+NOP-1)%NOP] = (i+NOP-1)%NOP;
        printf("%d put the chopsticks down\n", i);
        test(dp, (i+NOP-1)%NOP);
        test(dp, (i+1)%NOP);
    if (dp->next_count > 0){
        sem_post(dp->next);
    } else {
        sem_post(dp->mutex);
    }
}

void init (DP * dp){

    /**
     * sem_t * mutex;
     * sem_t * next;
     * int next_count;
     * int state[NOP];
     * Condition x[NOP];
     */
    
    sem_unlink("mutex");
    dp->mutex = sem_open("mutex", O_CREAT, 0660, 1);
    sem_unlink("next");
    dp->next = sem_open("next", O_CREAT, 0660, 0);

    dp->next_count = 0;

    for (int i = 0; i < NOP; i++){
        dp->state[i] = THINKING;
        //condition
        dp->x[i].count = 0;
        char * temp = uitoa(i);
        char * temp2 = strcat(temp, "cond");
        sem_unlink(temp2);
        dp->x[i].t = sem_open(temp2, O_CREAT, 0660, 0);
        //turn
        // dp->turn[i] = ((i/2)*2 + 2)%NOP;
    }
}

int main(){
    
    DP * dp = (DP *)mmap(NULL, sizeof(DP), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);

    init (dp);
    pid_t child [NOP];
    for (int i = 0; i < NOP; i++){
        child[i] = fork();
        if (child[i] < 0){
            perror("Forking failed\n");
            return 1;
        } else if (child[i] == 0){
            for (int j = 0; j < 100; j++){
                srand(getpid());
                sleep(rand()%10);
                pickup(dp, i);
                sleep(rand()%5);
                putdown(dp, i);
            }
            return 0;
        } else {
            1;
        }
    }

    int v;
    for (int i = 0; i < NOP; i++){
        waitpid(child[i], &v, 0);
    }
}