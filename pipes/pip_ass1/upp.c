#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define BUFFSIZE 50

int main(int argc, char const *argv[])
{
    int pp[2];
    pipe(pp);
    char buff[BUFFSIZE];
    if(argc == 1)
    {
        while(1)
        {
            read(0, buff, BUFFSIZE);
            printf("Final Proc : %s - O/P : %s\n", argv[0], buff);
            sleep(1);
        }
        return 0;
    }   

    pid_t p = fork();
    if(p > 0) {
        close(pp[0]);   
        while(1)
        {
            read(0, buff, BUFFSIZE);
            write(pp[1], buff, BUFFSIZE);
            sleep(1);
        }
        wait(NULL);
    } else if(p == 0) { 
        close(pp[1]);
        dup2(pp[0], 0);
        printf("Created Child : %s, %d\n", argv[1], argc);
        char *args[argc];// = (char*)calloc(argc, BUFFSIZE);
        for(int i=1; i<argc; i++)
        {
            char a[BUFFSIZE] = "./";
            strcat(a, argv[i]);
            args[i-1] = malloc(BUFFSIZE);
            strcpy(args[i-1], a);
            printf("%s\n", args[i-1]);
        }
        args[argc-1] = NULL;
        //printf("Creating child : %s\n", args[0]);
        execvp(args[0], args);
    }
    return 0;
}