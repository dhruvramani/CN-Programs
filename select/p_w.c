#include <fcntl.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc != 1)
        return printf("Invalid arguments\n");
    while(1)
    {
        printf("Process P%s\n", argv[1]);
        sleep(1);
    }
    return 0;
}
