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
    char* screen = "./fifo_screen";
    mkfifo(screen, 0666);

    int sfd = open(screen, O_RDWR);

    while(1)
    {
        char buff[20];
        int n = read(sfd, buff, 20);
        printf("%s\n", buff);
    }
}