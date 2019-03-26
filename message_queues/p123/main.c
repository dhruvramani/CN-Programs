#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>

struct mesg_buffer
{
    long mtype;
    char mtext[20];
} message, message1;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Error : Type process no. along please\n");
        return -1;
    }
    key_t k = ftok("key1", 65);
    int msqid = msgget(k, 0666 | IPC_CREAT);
    printf("%d -> %d\n", atoi(argv[1]), atoi(argv[2]));
    message.mtype = atoi(argv[1]);

    while(1)
    {
        sleep(1);
        strcpy(message.mtext, "FOO");
        msgsnd(msqid, &message, sizeof(message), 0);
        msgrcv(msqid, &message1, sizeof(message1), atoi(argv[2]), 0);
        printf("%s\n", message1.mtext);
    }
    return 0;
}
