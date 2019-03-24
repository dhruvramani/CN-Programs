#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    char file[] = "./sfifo";
    mkfifo(file, 0666);
    int fd = open(file, O_RDWR);
    char buff[100];
    while(1)
    {
        read(fd, buff, sizeof(buff));
        printf("%s\n", buff);
    }    
    return 0;
}
