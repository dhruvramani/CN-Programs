#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h> 

struct mesg_buffer
{
    long mtype;
    char mtext[10];
} message;

void* read_message()
{
    key_t key = ftok("keydede", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    while(1)
    {
        message.mtype = 1;
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        printf("%s", message.mtext);
        sleep(1);
    }
    msgctl(msgid, IPC_RMID, NULL);
}

void* write_message()
{
    key_t key = ftok("keydede", 65);
    int msgid = msgget(key, 066 | IPC_CREAT);
    for(int i=0; 1; i++)
    {
        message.mtype = 1;
        printf("Enter Message : ");
        read(0, message.mtext, sizeof(message.mtext));
        msgsnd(msgid, &message, sizeof(message), 0);
    }
    msgctl(msgid, IPC_RMID, NULL);
}

int main()
{
    pthread_t pid1, pid2;
    pthread_create(&pid1, NULL, read_message, NULL);
    pthread_create(&pid2, NULL, write_message, NULL);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    return 0;
}