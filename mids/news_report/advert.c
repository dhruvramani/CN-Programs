#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>

#define MAXLINE 4096
#define BUFFSIZE 100
#define LISTENQ 1024
#define SERV_PORT 9877

struct msg_buffer
{
    long msg_type;
    char msg_text[BUFFSIZE];
};

int main(int argc, char* argv[])
{
    key_t key = ftok("progfile", 'b');
    int msgid = msgget(key, 0666 | IPC_CREAT);
    struct msg_buffer message;

    while(1)
    {
        msgrcv(msgid, &message, sizeof(message), 2, 0);
        printf("%s\n", message.msg_text);
    }
    return 0;
}