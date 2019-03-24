#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <netinet/in.h>

int work = 1;

void startwork(int signo)
{
    work = 1;
}

void endwork(int signo)
{
    work = 0;
}

struct msgbuffer
{
    long type; 
    char text[100];
};

int main()
{
    int fd = open("./nreader2.txt", O_WRONLY);
    char pid[10];
    sprintf(pid, "%d", getpid());
    close(fd);
    write(fd, pid, sizeof(pid));

    signal(SIGUSR1, startwork);
    signal(SIGUSR2, endwork);

    int type = 2;
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    char sfifo[] = "./sfifo";
    mkfifo(sfifo, 0666);
    int fifo = open(sfifo, O_RDWR);
    struct msgbuffer msg;

    while(1)
    {
        if(work == 0)
            continue;
        
        if(msgrcv(msgid, &msg, sizeof(msg), type, 0))
        {
            char buff[100];
            strcpy(buff, msg.text);

            if(buff[0] > '0' && buff[0] < '9') // Live telecast
            {
                int port = atoi(buff);

                int sfd = socket(AF_INET, SOCK_STREAM, 0);
                struct sockaddr_in telecast;
                bzero(&telecast, sizeof(telecast));
                telecast.sin_family = AF_INET;
                telecast.sin_addr.s_addr = inet_addr("127.0.0.1");
                telecast.sin_port = htons(5000);

                int ret = connect(sfd, (struct sockaddr*)&telecast, sizeof(telecast));
                if(ret == -1)
                    continue;
                
                work = 0;
                fd = open("./nreader1", O_RDWR);
                memset(pid, '\0', sizeof(pid));
                read(fd, pid, sizeof(pid));
                int foo = atoi(pid);
                kill((pid_t)foo, SIGUSR2);

                while(1)
                {
                    char buffer[100];
                    if(buffer[0] == '0')
                        break;
                    read(sfd, buffer, sizeof(buffer));
                    write(fifo, buffer, sizeof(buffer));
                }

                kill((pid_t)foo, SIGUSR1);
                work = 1;

                fd = open("./editor", O_RDWR);
                read(fd, pid, sizeof(pid));
                foo = atoi(pid);
                kill((pid_t)foo, SIGUSR1);
            } else {
                write(fifo, buff, sizeof(buff));
            }

        }
    }
}