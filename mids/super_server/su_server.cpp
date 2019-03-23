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
#define BUFFSIZE 8192
#define LISTENQ 1024
#define SERV_PORT 9877

int curr1, curr2;

struct threadargs
{
    int nsfd;
};

static void sig_handler1(int signo)
{
    curr1 --;
}

void* thread_func(void* args)
{
    printf("Service 2\n");
    int nsfd = ((struct threadargs*)args)->nsfd;
    char buff[MAXLINE], str[10];
    recv(nsfd, buff, MAXLINE, 0);
    sprintf(str, "%d", strlen(buff));
    send(nsfd, str, strlen(str) + 1, 0);
    curr2--;
}

int main(int argc, char** argv)
{
    signal(SIGUSR1, sig_handler1);

    int sfd1, sfd2;
    struct sockaddr_in servip1, servip2;
    char buff[MAXLINE];

    curr1=0; curr2=0;

    sfd1 = socket(AF_INET, SOCK_STREAM, 0);
    sfd2 = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servip1, sizeof(servip1));
    servip1.sin_family = AF_INET;
    servip1.sin_addr.s_addr = inet_addr("127.0.0.1");
    servip1.sin_port = htons(5000);

    bzero(&servip2, sizeof(servip2));
    servip2.sin_family = AF_INET;
    servip2.sin_addr.s_addr = inet_addr("127.0.0.1");
    servip2.sin_port = htons(5001);

    bind(sfd1, (struct sockaddr*)&servip1, sizeof(servip1));
    bind(sfd2, (struct sockaddr*)&servip2, sizeof(servip2));

    listen(sfd1, LISTENQ);
    listen(sfd2, LISTENQ);

    fd_set rset;
    
    char addr[20];
    inet_ntop(AF_INET, &(servip1.sin_addr), addr, 20);
    printf("%s\n", addr);

    inet_ntop(AF_INET, &(servip2.sin_addr), addr, 20);
    printf("%s\n", addr);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(sfd1, &rset);
        FD_SET(sfd2, &rset);

        if(select(FD_SETSIZE, &rset, NULL, NULL, &tv))
        {   
            if(FD_ISSET(sfd1, &rset))
            {
                if(curr1 <= 25)
                {
                    int nsfd = accept(sfd1, (struct sockaddr*)NULL, NULL);

                    printf("%d\n", nsfd);
                    curr1++;
                
                    if(nsfd > 0)
                    {
                        if(fork()==0)
                        {   
                            close(sfd1);
                            int newstdin = dup(fileno(stdin));
                            int newstdout = dup(fileno(stdout));
                            char arg1[5], arg2[5], arg3[5];

                            sprintf(arg1, "%d", newstdin);
                            sprintf(arg2, "%d", newstdout);
                            sprintf(arg3, "%d", nsfd);

                            char* args[] = {"./service1", arg1, arg2, arg3, NULL};
                            execv(args[0], args);
                        }
                        else
                        {
                            close(nsfd);
                        }
                    }
                }
            }
            else if(FD_ISSET(sfd2, &rset))
            {
                if(curr2<=15)
                {
                    int nsfd = accept(sfd2, (struct sockaddr*)NULL, NULL);

                    curr2++;
                    pthread_t tid;
                    struct threadargs* args = (threadargs *) malloc(sizeof(struct threadargs));
                    args->nsfd = nsfd;
                    pthread_create(&tid, NULL, thread_func, (void*)args);
                }
            }
        }
    }
    return 0;
}