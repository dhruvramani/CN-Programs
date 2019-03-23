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

int main(int argc, char** argv)
{
    int sfd, n;
    char buff[MAXLINE+1];
    struct sockaddr_in servip;

    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("Socket Error\n");

    bzero(&servip, sizeof(servip));
    servip.sin_family = AF_INET;
    servip.sin_port = htons(5000);
    servip.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sfd, (struct sockaddr*)&servip, sizeof(servip)) < 0)
        printf("Connection Error\n");

    scanf("%s", buff);
    write(sfd, buff, strlen(buff) + 1);
    read(sfd, buff, MAXLINE);
    printf("%s", buff);

    return 0;
}