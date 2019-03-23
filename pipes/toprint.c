#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define BUFFSIZE 50

int main()
{
    char buff[BUFFSIZE];
    read(0, buff, BUFFSIZE);
    printf("P2 : %s\n", buff);
    return 0;
}