#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
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

    int ret = bind(sfd, (struct sockaddr*)&serv, sizeof(serv));
    listen(sfd, 100);
    while(1)
    {
        char buff[] = "Hello from the other side\n";
        int nsfd = accept(sfd, (struct sockaddr*)NULL, NULL);
        for(int i=0; i<10; i++)
            write(sfd, buff, sizeof(buff));
    }
    return 0;
}