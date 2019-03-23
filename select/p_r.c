#include <fcntl.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h>
#include<string.h>

int main()
{
    char buff[100];
    while(1)
    {
        scanf("%s", &buff);
        write(1, buff, strlen(buff) + 1);
    }
    return 0;
}
