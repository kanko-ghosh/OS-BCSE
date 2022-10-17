#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * itoa(int num){
    char * str;
    str = (char *)malloc(5 * sizeof(char));
    str[4] = 0;
    for (int i = 3; i >= 0; i--){
        str[i] = (num%10)+'0';
        num/=10;
    }
    return str;
}

int main ()
{
    char * temp = itoa(25);
    char * temp2 = itoa(35);
    printf("%s", strcat(temp, temp2));
    free(temp);
}