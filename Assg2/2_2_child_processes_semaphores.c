#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

#define NUM_ITER 10
#define MAX_SLEEP_TIME 0.100 // in seconds
#define NEWL printf("\n")

typedef char bool;
bool true = 1;
bool false = 0;

void swap(char *c1, char *c2)
{
    char *tmp = c1;
    c1 = c2;
    c2 = tmp;
}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap((str+start), (str+end));
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}


int main()
{
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    NEWL;
    srand(time(0));
    char buffer[33]; // for itoa

    // Semaphores - sem[i][j] signals jth iteration of ith process is complete
    // The last iterations dont need to signal anyone
    /* sem_t **sem = (sem_t **)malloc(n * sizeof(sem_t *));
    int semIndex = 0;
    for(int i = 0; i < n; i++) sem[i] = (sem_t *)malloc(NUM_ITER * sizeof(sem_t));
    for(int i = 0; i < n; i++) for(int j = 0; j < NUM_ITER; j++) sem_init(&sem[i][j], ++semIndex, 0);  // non-zero pshared means shared among processes */
    sem_t ***sem = (sem_t ***)malloc(n * sizeof(sem_t **));
    for(int i = 0; i < n; i++) sem[i] = (sem_t **)malloc(NUM_ITER * sizeof(sem_t *));
    int semIndex = 0;
    for(int i = 0; i < n; i++) for(int j = 0; j < NUM_ITER; j++) sem[i][j] = sem_open(itoa(++semIndex, buffer, 10), O_CREAT, 0660, 0);

    pid_t *child = (pid_t *)malloc(n * sizeof(pid_t));
    int parentPid = getpid();
    printf("Parent Process PID: %d\n\n\n\n", parentPid);
    printf("Parent PID\tChild PID\tIter Number\n\n");

    for(int i = 0; i < n; i++)
    {
        child[i] = fork();

        if(child[i] < 0)
        {
            printf("Fork #%d failed, exiting ...\n", i);
            return 1;
        }
        else if(child[i] == 0)
        {
            // task for our child process
            for(int j = 1; j <= NUM_ITER; j++)
            {
                if(i != 0) sem_wait(sem[i - 1][j - 1]); // first process does not need to wait for anyone

                printf("%d\t\t%d\t\t%d\n", getppid(), getpid(), j);

                if(i != n - 1) sem_post(sem[i][j - 1])/*, printf("%d %d posted ...\n", i, j - 1)*/; // 0-indexed

                ++
            }

            break; // no need of forking from child like linked list, we have tree structure
        }
        else
        {
            // returns pid of child to parent process
            printf("\t\t---------- Child Process #%d created (pid: %d) ----------\n", i, child[i]);
        }
    }

    if(getpid() == parentPid)
    {
        int stat;
        for(int i = 0; i < n; i++) waitpid(child[i], &stat, 0);


        free(child);
        /* for(int i = 0; i < n; i++) for(int j = 0; j < NUM_ITER; j++) sem_destroy(&sem[i][j]);
        for(int i = 0; i < n; i++) free(sem[i]);
        free(sem); */
        for(int i = 0; i < n; i++) for(int j = 0; j < NUM_ITER; j++) sem_unlink(itoa(--semIndex, buffer, 10));
        for(int i = 0; i < n; i++) free(sem[i]);
        free(sem);
    }

    return 0;
}
