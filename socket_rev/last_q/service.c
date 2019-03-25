#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct threadargs
{
    int nsfd;
};

void* iofunc(void *argo)
{
    struct threadargs *args = (struct threadargs*)argo;
    int nsfd = args->nsfd;
    for(int i=0; i<50; i++)
    {
        char buff[] = "Hi from Service!";
        write(nsfd, buff, sizeof(buff));
    }
}

int main(int argc, char* argv[])
{
    int port = atoi(argv[1]);
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    int ret = bind(sfd, (struct sockaddr*)server, sizeof(server));
    if(ret == -1)
        return -1;
    listen(sfd, 50);

    while(1)
    {
        int nsfd = accept(sfd, (struct sockaddr*)NULL, NULL);
        struct threadargs* args = (struct threadargs*) malloc(sizeof((struct threadargs)));
        args->nsfd = nsfd;
        pthread_t t;
        pthread_create(&t, NULL, iofunc, (void*) args);
        pthread_join(t, NULL);
    }
}