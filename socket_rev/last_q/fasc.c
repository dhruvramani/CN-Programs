#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX 10

struct socks
{
    int sfd, port;
    char path[100];
};

struct threadargs
{
    int nsfd;
    int servsfd;
};

void* iofunc(void *argo)
{
    struct threadargs* args = (struct threadargs*) argo;
    int nsfd = args->nsfd;
    int servsfd = args->servsfd;

    char buff[100];
    int n;
    while((n = read(servsfd, buff, sizeof(buff))) > 0)
        write(nsfd, buff, sizeof(buff));
}

int main()
{
    struct socks sfds[MAX + 1];

    // For the info one 
    struct socks init; 
    int connect_info = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in info;
    bzero(&info, sizeof(info));
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(5020);
    int ret = bind(connect_info, (struct sockaddr*)&info, sizeof(info));
    if(ret == -1)
        return -1;
    listen(connect_info, 50);
    init.sfd = connect_info;
    init.port = 5020;
    strcpy(init.path, " ");
    sfds[0] = init;

    int n;
    printf("Enter number of services\n");
    scanf("%d", &n);

    for(int i=1; i < n+1; i++)
    {
        int port;
        char path[100] = "";
        printf("Enter Port\n");
        scanf("%d", &port);
        printf("Enter Path\n");
        scanf("%s", path);

        struct socks service;
        service.port = port;
        strcpy(service.path, path);
        sfds[i].sfd = 0;
        sfds[i] = service;

        sfds[i].sfd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(port);
        int f = bind(sfds[i].sfd, (struct sockaddr*)&addr, sizeof(addr));
        listen(sfds[i].sfd, 50);
        pid_t p = fork();
        
        if(p == 0)
        {
            char ports[10] = "";
            sprintf(ports, "%d", port + MAX + 1); // PORT + MAX + 1
            char *args[] = {path, ports, NULL};
            printf("%s executing\n", path);
            execvp(args[0], args);
        } 
    }

    fd_set rset;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    for(int i=0; i<n+1; i++)
        printf("%d\n", sfds[i].port);

    while(1)
    {
        FD_ZERO(&rset);
        for(int i=0; i<n + 1; i++)
            FD_SET(sfds[i].sfd, &rset);

        if(select(FD_SETSIZE, &rset, NULL, NULL, &tv))
        {
            printf("Selected\n");
            for(int i=0; i<n + 1; i++)
                if(FD_ISSET(sfds[i].sfd, &rset))
                {
                    printf("%d accepted\n", i);
                    int nsfd = accept(sfds[i].sfd, (struct sockaddr*)NULL, NULL);
                    if(i == 0)
                        write(nsfd, (void*)& sfds, sizeof(sfds)); // Send a pointer to the struct
                    else {
                        int sockfoo = socket(AF_INET, SOCK_STREAM, 0);
                        struct sockaddr_in servo;
                        bzero(&servo, sizeof(servo));
                        servo.sin_family = AF_INET;
                        servo.sin_addr.s_addr = inet_addr("127.0.0.1");
                        servo.sin_port = htons(sfds[i].port + MAX + 1);
                        int roo = connect(sockfoo, (struct sockaddr*)&servo, sizeof(servo));
                        printf("Connected to service\n");
                        pthread_t t;
                        struct threadargs *args = (struct threadargs*) malloc(sizeof(struct threadargs));
                        args->nsfd = nsfd;
                        args->servsfd = roo;
                        pthread_create(&t, NULL, iofunc, (void *)args);
                        pthread_join(t, NULL);
                    }
                }
        }
    }

    return 0;
}