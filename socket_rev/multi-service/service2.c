#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    printf("Connected to service 1\n");
    while(1)
    {
        printf("Service 2 sent hi!\n");
    }
    return 0;
}