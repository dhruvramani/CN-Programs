#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

int main(int args, char* argv[])
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5000);
    int ret = connect(sfd, (struct sockaddr*) &server, sizeof(server));
    printf("%d\n", ret);

    while(1)
    {
        char buff[100];
        strcpy(buff, "Client");
        write(sfd, buff, sizeof(buff));
        read(sfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
    }
    close(sfd);
    return 0;
}