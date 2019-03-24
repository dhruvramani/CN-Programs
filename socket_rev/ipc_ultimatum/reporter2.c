#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    char filename[] = "./r2fifo";
    mkfifo(filename, 0666);
    int fd = open(filename, O_RDWR);

    while(1)
    {
        char buff[50];
        scanf("%s", buff);
        write(fd, buff, sizeof(buff));
    }
    return 0;
}