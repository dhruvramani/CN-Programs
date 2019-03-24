#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    // Socket 1
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5000);

    int ret = bind(sfd, (struct sockaddr*)&server, sizeof(server));
    if(ret == -1)
    {
        printf("Bind1\n");
        return -1;
    }
    listen(sfd, 50);

    // Socket 2
    int sfd2 = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server2;
    bzero(&server2, sizeof(server2));
    server2.sin_family = AF_INET;
    server2.sin_addr.s_addr = inet_addr("127.0.0.1");
    server2.sin_port = htons(5001);

    ret = bind(sfd, (struct sockaddr*)&server2, sizeof(server2));
    if(ret == -1)
    {
        printf("Bind2\n");
        return -1;
    }

    listen(sfd2, 50);

    fd_set rset;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    while(1)
    {
        // For each incoming client
        FD_ZERO(&rset);
        FD_SET(sfd, &rset);
        FD_SET(sfd2, &rset);

        if(select(FD_SETSIZE, &rset, NULL, NULL, &tv))
        {
            if(FD_ISSET(sfd, &rset))
            {
                struct sockaddr_in cli;
                bzero(&cli, sizeof(cli));
                int size = sizeof(cli);

                int nsfd = accept(sfd, (struct sockaddr*)&cli, &size);
                pid_t p = fork();
                if(p == 0)
                {
                    close(sfd);
                    dup2(nsfd, 1);
                    char* args[] = {"./service1", NULL};
                    execvp(args[0], args);
                } else if(p > 0)
                    close(nsfd);
            }

            if(FD_ISSET(sfd2, &rset))
            {
                struct sockaddr_in cli;
                bzero(&cli, sizeof(cli));
                int size = sizeof(cli);
                int nsfd = accept(sfd2, (struct sockaddr*)&cli, &size);
                pid_t p = fork();
                if(p == 0)
                {
                    close(sfd2);
                    dup2(nsfd, 1);
                    char *args[] = {"./service2", NULL};
                    execvp(args[0], args);
                } else if(p > 0)
                    close(nsfd);
            }
        }
        FD_ZERO(&rset);
    }
}