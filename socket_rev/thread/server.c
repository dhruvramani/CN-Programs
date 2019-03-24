#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct threadargs
{
    int sfd;
};

void* handler(void* sfdarg)
{
    struct threadargs *arg = (struct threadargs*)sfdarg;
    int nsfd = arg->sfd;
    while(1)
    {
        char buff[100];
        strcpy(buff, "Hello from server");
        write(nsfd, buff, sizeof(buff));
    }
}

int main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5000);

    int ret = bind(sfd, (struct sockaddr*)&serv, sizeof(serv));
    if(ret == -1)
        return ret;
    listen(sfd, 50);

    while(1) // for each client
    {
        struct sockaddr_in cli;
        int size = sizeof(cli);
        bzero(&cli, sizeof(cli));
        int nsfd = accept(sfd, (struct sockaddr*)&cli, &size);
        pthread_t t;
        struct threadargs* targs = (struct threadargs*) malloc(sizeof(struct threadargs));
        targs->sfd = nsfd;
        pthread_create(&t, NULL, handler, (void*)targs);
        pthread_join(t, NULL);
    }

}