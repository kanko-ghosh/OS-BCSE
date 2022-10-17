#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>


#define BUFFER_SIZE 1024

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
    int fd, n, numBytes;
    char *myFifo = "/tmp/myFifo";
    mkfifo(myFifo, 0666);
    char buffer[BUFFER_SIZE];

    printf("Enter number of receivers: ");
    scanf("%d", &n);
    char tmpBuffer[33];


    // Semaphores
    sem_t *semFull[n];
    for(int i = 0; i < n; i++)
    {
        sem_unlink(itoa(i, tmpBuffer, 10));
        if((semFull[i] = sem_open(itoa(i, tmpBuffer, 10), O_CREAT, 0660, 0)) == SEM_FAILED){ printf("Sem Failed (%d) !!!\n", i); return 1; }
    }

    sem_t *semCnt, *semCheckNext;
    sem_unlink("cnt"), sem_unlink("chk");
    if((semCnt = sem_open("cnt", O_CREAT, 0660, 0)) == SEM_FAILED){ printf("Sem Failed in cnt !!!\n"); return 1; }
    if((semCheckNext = sem_open("chk", O_CREAT, 0660, 0)) == SEM_FAILED){ printf("Sem Failed in chk !!!\n"); return 1; }




    printf("\nYou are the WEATHER MASTER !!!\n");
    printf("Let everyone know about the weather :)  ...\n");
    printf("Enter \"!end\" to end (without quotes) ...\n\n");

    int curIter = 0;
    do
    {
        curIter++;
        for(int i = 0; i < n; i++) sem_post(semFull[i]);

        printf("Enter your message [#%d]: ", curIter);
        // fflush(stdin);
        // scanf("%[^\n]s", buffer);
        scanf("%s", buffer);
        // getchar();  // to remove dummy newline

        fd = open(myFifo, O_WRONLY);  // write only mode
        // If I am not using the return of write function, fifo not properly written or read | WHY ??? ----------------------------------------------------------------------
        // for(int i = 0; i < n; i++) numBytes = write(fd, buffer, strlen(buffer) + 1); /* printf("lol2 | %d\n", write(fd, buffer, strlen(buffer) + 1)); */ // +1 to account for '\0'
        for(int i = 0; i < n; i++) numBytes = write(fd, buffer, BUFFER_SIZE); /* printf("lol2 | %d\n", write(fd, buffer, strlen(buffer) + 1)); */ // +1 to account for '\0'
        close(fd);

        // printf("lol3\n");        while(true)
        {
            // printf("lol4\n");
            sem_wait(semCheckNext);
            int val;
            sem_getvalue(semCnt, &val);
            if(val == curIter * n) break;
        }
    }
    while(strcmp(buffer, "!end"));

    printf("\n\nMay the weather be with you ...\n");
    printf("Program ended ...\n");

    sem_unlink("cnt"), sem_unlink("chk");  // printf("su\n");
    for(int i = 0; i < n; i++) sem_unlink(itoa(i, tmpBuffer, 10)), sem_destroy(semFull[i]);
    sem_destroy(semCnt), sem_destroy(semCheckNext);  // printf("sd\n");

    return 0;
}
