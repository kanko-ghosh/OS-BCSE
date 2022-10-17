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
    int fd, n, lol;
    char *myFifo = "/tmp/myFifo";
    mkfifo(myFifo, 0666);
    char buffer[BUFFER_SIZE];

    printf("Enter receiver number: ");
    scanf("%d", &n);
    char tmpBuffer[33];


    // Semaphores - Opening already created semaphores
    sem_t *semFull, *semCnt, *semCheckNext;
    if((semFull = sem_open(itoa(n, tmpBuffer, 10), 0)) == SEM_FAILED){ printf("Sem Failed in full !!!\n"); return 1; }
    if((semCnt = sem_open("cnt", 0)) == SEM_FAILED){ printf("Sem Failed in cnt !!!\n"); return 1; }
    if((semCheckNext = sem_open("chk", 0)) == SEM_FAILED){ printf("Sem Failed in chk !!!\n"); return 1; }




    printf("We listen to the WEATHER MASTER !!!\n");
    printf("Let us everyone know about the weather :)  ...\n\n");

    // sem_getvalue(semFull, &lol);
    // printf("%d\n", lol);

    do
    {
        sem_wait(semFull);

        fd = open(myFifo, O_RDONLY);  // read only mode
        read(fd, buffer, BUFFER_SIZE);
        printf("Message received: %s\n", buffer);
        close(fd);

        sem_post(semCnt);
        sem_post(semCheckNext);
    }
    while(strcmp(buffer, "!end"));

    printf("\n\nMay the weather be with you ...\n");
    printf("\t--- END OF TRANSMISSION ---\n");
    printf("Program ended ...\n");

    sem_destroy(semFull);
    sem_destroy(semCnt);
    sem_destroy(semCheckNext);

    return 0;
}
