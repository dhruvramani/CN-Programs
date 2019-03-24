#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>

int livecast = 0;

void handler(int signo)
{
    livecast += 1;
}

struct msgbuff
{
    long type;
    char text[100];
};

int main()
{
    char fname = "./editorpid.txt", pido[10];
    int fd = open(fname, O_RDWR);
    sprintf(pido, "%d", getpid());
    write(fd, pido, sizeof(pido));
    signal(SIGUSR1, handler);
    char rep1[] = "./r1fifo", rep2[] = "./r2fifo";
    mkfifo(rep1, 0666);
    mkfifo(rep2, 0666);
    int fd1 = open(rep1, O_RDWR), fd2 = open(rep2, O_RDWR), alt = 1;

    fd_set rset;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(fd1, &rset);
        FD_SET(fd2, &rset);

        if(select(FD_SETSIZE, &rset, NULL, NULL, &tv))
        {
            printf("Live Cast %d\n", livecast);
            if(FD_ISSET(fd1, &rset))
            {
                char buff[100];
                read(fd1, buff, sizeof(buff));
                struct msgbuff msg;
                msg.type = alt;
                strcpy(msg.text, buff);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                if(alt == 1)
                    alt = 2;
                else
                    alt = 1;
            }
            if(FD_ISSET(fd2, &rset))
            {
                char buff[100];
                read(fd2, buff, sizeof(buff));
                struct msgbuff msg;
                msg.type = alt;
                strcpy(msg.text, buff);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                if(alt == 1)
                    alt = 2;
                else
                    alt = 1;
            }
        }
    }
}