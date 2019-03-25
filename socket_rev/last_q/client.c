#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5001);

    int ret = connect(sfd, (struct sockaddr*)&serv, sizeof(serv));
    while(1)
    {
        char buff[100];
        read(sfd, buff, sizeof(buff));
        printf("%s\n", buff);
    }
    return 0;
}