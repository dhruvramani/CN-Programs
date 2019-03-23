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

void read_writefd(int fd, int msgid, int dfd, int *read_flag) // void read_writefd(int fd, int nfd1, int nfd2, int dfd, int *read_flag)
{
    char buff[20];
    int n = read(fd, buff, 20);

    printf("%s\n", buff);

    if(buff[0]=='/' && buff[1]=='d') // According to Question, if /d send it to document writter
    {    
        write(dfd, buff, n);
        return ;
    }

    struct msg_buffer message;
    if(buff[0]=='/' && buff[1]=='a')
    {
        message.msg_type = 2;
        strcpy(message.msg_text, buff);
        msgsnd(msgid, &message, sizeof(message), 0);
        return ;
    }
    
    message.msg_type = read_flag;
    strcpy(message.msg_text, buff);
    // Alternate
    if(read_flag==0)
    { msgsnd(msgid, &message, sizeof(message), 0); read_flag = 1; }
    else
    {  msgsnd(msgid, &message, sizeof(message), 0); read_flag = 0; }

}

int main(int argc, char* argv[])
{
    char* pathname1 = "./fifo_rep_1";
    mkfifo(pathname1, 0666);
    char* pathname2 = "./fifo_rep_2";
    mkfifo(pathname2, 0666);
    char* pathname3 = "./fifo_rep_3";
    mkfifo(pathname3, 0666);
    
    //char* pathname4 = "./fifo_editor_1";
    //mkfifo(pathname4, 0666);
    //char* pathname5 = "./fifo_editor_2";
    //mkfifo(pathname5, 0666);

    // Replacing FIFO's by message queues
    key_t key = ftok("progfile", 'b');
    int msgid = msgget(key, 0666 | IPC_CREAT);

    char* pathname6 = "./fifo_doc_1";
    mkfifo(pathname6, 0666);

    int fd1, fd2, fd3, dfd; //, nfd1, nfd2;

    printf("Before opening\n");
    // Create FIFOs for all reporters and send to readers using FIFOs
    fd1 = open(pathname1, O_RDWR);
    fd2 = open(pathname2, O_RDWR);
    fd3 = open(pathname3, O_RDWR);
    //nfd1 = open(pathname4, O_RDWR);
    //nfd2 = open(pathname5, O_RDWR);
    dfd = open(pathname6, O_RDWR);

    printf("After opening\n");

    fd_set rset;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int read_flag = 0;

    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(fd1, &rset);
        FD_SET(fd2, &rset);
        FD_SET(fd3, &rset);

        if(select(FD_SETSIZE, &rset, NULL, NULL, &tv)) // Select
        {   
            printf("After select\n");
            if(FD_ISSET(fd1, &rset))
                read_writefd(fd1, msgid, dfd, &read_flag); // read_writefd(fd1, nfd1, nfd2, dfd, &read_flag);
            else if(FD_ISSET(fd2, &rset))
                read_writefd(fd2, msgid, dfd, &read_flag); // read_writefd(fd2, nfd1, nfd2, dfd, &read_flag);
            else if(FD_ISSET(fd3, &rset))
                read_writefd(fd3, msgid, dfd, &read_flag); // read_writefd(fd3, nfd1, nfd2, dfd, &read_flag);

        }
    }

    return 0;
}