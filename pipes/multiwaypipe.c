#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define BUFFSIZE 10

int main()
{
    int pp1[2], pp2[2];
    pipe(pp1);
    pipe(pp2);
    char buff[BUFFSIZE];
    pid_t p = fork();
    if(p == 0)
    {
        close(pp1[1]);
        close(pp2[0]); // closes because they aren't in the hand of the child
        while(1)
        {
            read(pp1[0], buff, BUFFSIZE);
            printf("Child : %s\n", buff);
            sleep(1);
            strcpy(buff, "from c");
            write(pp2[1], buff, BUFFSIZE);
        }
    } else if(p > 0) {
        close(pp1[0]);
        close(pp2[1]);
        while(1)
        {
            strcpy(buff, "from p");
            write(pp1[1], buff, BUFFSIZE);
            sleep(1);
            read(pp2[0], buff, BUFFSIZE);
            printf("Parent : %s\n", buff);
        }
        wait(NULL);
    }
    return 0;
}