#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    char filename[] = "./r1fifo";
    mkfifo(filename, 0666);
    int fd = open(filename, O_WRONLY);
    while(1)
    {
        char buff[50];
        scanf("%s", buff);
        write(fd, buff, sizeof(buff));
    }
}