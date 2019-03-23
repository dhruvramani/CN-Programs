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
    int pp[2];
    char buff[BUFFSIZE];
    pipe(pp);
    int p = fork();
    if(p > 0)
    {
        close(pp[0]); // parent doesn't have control over read
        strcpy(buff, "from parent");
        write(pp[1], buff, BUFFSIZE);
        wait(NULL);
    } else if(p == 0)
    {
        close(pp[1]);
        dup2(pp[0], 0);
        char *args[] = {"./toprint", NULL};
        execv(args[0], args);
    }
}