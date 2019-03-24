#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5000);

    int ret = connect(sfd, (struct sockaddr*)&serv, sizeof(serv));
    if(ret == -1)
        return -1;
    printf("Client connected\n");
    char buff[100];
    int n;
    while((n = read(sfd, buff, sizeof(buff))) > 0)
        printf("%s\n", buff);
    return 0;
}