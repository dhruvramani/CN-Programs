#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

struct mesg_buffer
{
    long mtype;
    char mtext[20];
} message;

int main()
{
    srand(time(NULL));
    key_t k = ftok("key1", 65);
    int msqid = msgget(k, 0666 | IPC_CREAT);
    int group_id = rand() % 6;
    sprintf(message.mtext, "%d|%d|%s", getpid(), group_id, "add");
    message.mtype = 1;
    msgsnd(msqid, &message, sizeof(message), 0);

    sleep(2);

    while(1)
    {
        sprintf(message.mtext, "%d|%d|%s", getpid(), group_id, "foo");
        message.mtype = 1;
        msgsnd(msqid, &message, sizeof(message), 0);
        msgrcv(msqid, &message, sizeof(message), getpid(), 0);
        printf("%s\n", message.mtext);
        sleep(2);
    }

    return 0;
}