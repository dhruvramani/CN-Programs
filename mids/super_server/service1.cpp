#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define MAXLINE 4096

int main(int argc, char* argv[])
{
    printf("Service 1\n");
    int newstdin = atoi(argv[1]), newstdout = atoi(argv[2]), cfd = atoi(argv[3]);

    char buff[MAXLINE];

    printf("%d %d\n", newstdin, newstdout);
    read(cfd, buff, MAXLINE);
    printf("%s\n", buff);

    read(newstdin, buff, MAXLINE);
    write(cfd, buff, strlen(buff)+1);

    close(cfd);
    kill(getppid(), SIGUSR1);
    return 0;
}