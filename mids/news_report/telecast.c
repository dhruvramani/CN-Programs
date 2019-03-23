#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>

#define MAXLINE 4096
#define BUFFSIZE 8192
#define LISTENQ 1024
#define SERV_PORT 9877

int main(int argc, char* argv[])
{
    int lfd, cfd;
    struct sockaddr_in servip;
    char buff[MAXLINE];
    time_t ticks;

    lfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servip, sizeof(servip));
    servip.sin_family = AF_INET;
    servip.sin_addr.s_addr = inet_addr("127.0.0.1");
    servip.sin_port = htons(5002);

    bind(lfd, (struct sockaddr*)&servip, sizeof(servip));
    listen(lfd, LISTENQ);

    char addr[20];
    inet_ntop(AF_INET, &(servip.sin_addr), addr, 20);

    printf("%s\n", addr);

    while(1)
    {
        cfd = accept(lfd, (struct sockaddr*)NULL, NULL);
        scanf("%s", buff);
        write(cfd, buff, strlen(buff));
        close(cfd);
    }
}