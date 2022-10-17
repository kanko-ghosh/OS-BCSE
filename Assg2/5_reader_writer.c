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
    int r, w, rIter;  // Take input r = 10 , w = 10 , rIter = 20 for a good idea ...
    char itoaBuffer[33]; // for itoa
    // Shared memory ...
    int *buffer = (int *)mmap(NULL , sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    *buffer = -1;  // Initial value
    int *reader_cnt = (int *)mmap(NULL , sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    *reader_cnt = 0;  // Initial value
    int *writer_queued = (int *)mmap(NULL , sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    *reader_cnt = 0;  // Initial value

    printf("Enter #readers: ");
    scanf("%d", &r);
    printf("Enter #writers: ");
    scanf("%d", &w);
    printf("Enter #times a reader reads: ");
    scanf("%d", &rIter);

    // Semaphores ...
    sem_unlink("rw_mutex"), sem_unlink("r_mutex"), sem_unlink("w_mutex"), sem_unlink("writer_done");
    sem_t *rw_mutex, *r_mutex, *w_mutex, *writer_done[r];  // every reader needs to know writing is done
    if((rw_mutex = sem_open("rw_mutex", O_CREAT, 0660, 1)) == SEM_FAILED){ printf("Sem failed rw_mutex !!!\n"); return 1; }
    if((r_mutex = sem_open("r_mutex", O_CREAT, 0660, 1)) == SEM_FAILED){ printf("Sem failed r_mutex !!!\n"); return 1; }
    if((w_mutex = sem_open("w_mutex", O_CREAT, 0660, 1)) == SEM_FAILED){ printf("Sem failed w_mutex !!!\n"); return 1; }
    for(int i = 0; i < r; i++) if((writer_done[i] = sem_open(itoa(i, itoaBuffer, 10), O_CREAT, 0660, 0)) == SEM_FAILED){ printf("Sem failed writer_done !!!\n"); return 1; }


    pid_t reader[r], writer[w];

    for(int i = 0; i < r; i++)
    {
        reader[i] = fork();

        if(reader[i] < 0)
        {
            printf("Fork reader #%d failed, exiting ...\n", i);
            return 1;
        }
        else if(reader[i] == 0)
        {
            // Reader Subprocess
            while(rIter--)
            {
                // On removing the below line, we go back to case where writers are not given priority over readers ...
                // Remove the below line to see writers not updating almost immediately after they are started ...
                while(*writer_queued > 0) sem_wait(writer_done[i]);

                sem_wait(r_mutex);
                *reader_cnt = *reader_cnt + 1;
                if(*reader_cnt == 1) sem_wait(rw_mutex);
                sem_post(r_mutex);

                printf("[ Reader #%d reads data = %d ]\n", i, *buffer);

                sem_wait(r_mutex);
                *reader_cnt = *reader_cnt - 1;
                if(*reader_cnt == 0) sem_post(rw_mutex);
                sem_post(r_mutex);
            }

            return 0;
        }
        else
        {
            // returns pid of child to parent process
            printf("\t\t---------- Reader Process #%d created (pid: %d) ----------\n", i, reader[i]);
        }
    }

    for(int i = 0; i < w; i++)
    {
        writer[i] = fork();

        if(writer[i] < 0)
        {
            printf("Fork writer #%d failed, exiting ...\n", i);
            return 1;
        }
        else if(writer[i] == 0)
        {
            // Writer Subprocess
            sem_wait(w_mutex);
            *writer_queued = *writer_queued + 1;
            sem_post(w_mutex);

            sem_wait(rw_mutex);
            *buffer = i;
            printf("Writer #%d wrote data = %d ...\n", i, i);
            sem_post(rw_mutex);

            sem_wait(w_mutex);
            *writer_queued = *writer_queued - 1;
            sem_post(w_mutex);

            for(int j = 0; j < r; j++) sem_post(writer_done[j]);
            return 0;
        }
        else
        {
            // returns pid of child to parent process
            printf("\t\t---------- Writer Process #%d created (pid: %d) ----------\n", i, writer[i]);
        }
    }


    int stat;
    // printf("lol\n");
    for(int i = 0; i < r; i++) waitpid(reader[i], &stat, 0);  // printf("lol2\n");
    for(int i = 0; i < w; i++) waitpid(writer[i], &stat, 0);  // printf("lol1\n");

    sem_unlink("rw_mutex"), sem_unlink("r_mutex"), sem_unlink("w_mutex");
    for(int i = 0; i < r; i++) sem_unlink(itoa(i, itoaBuffer, 10));
    sem_destroy(rw_mutex), sem_destroy(r_mutex), sem_destroy(w_mutex);
    for(int i = 0; i < r; i++) sem_destroy(writer_done[i]);


    return 0;
}
