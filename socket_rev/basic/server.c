#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <string.h>
#include <strings.h>


int main(int argc, char* argv[])
{

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ss;
    bzero(&ss, sizeof(ss));
    ss.sin_family = AF_INET;
    ss.sin_addr.s_addr = inet_addr("127.0.0.1");
    ss.sin_port = htons(5000);

    int ret = bind(sfd, (struct sockaddr*)& ss, sizeof(ss));
    if(ret == -1)
        return -1;
    listen(sfd, 50);

    while(1)
    {
        struct sockaddr_in cli;
        int size;
        bzero(&cli, sizeof(cli));
        int nsfd = accept(sfd, (struct sockaddr*) &cli, &size);
        //pid_t p = fork();
        //if(p == 0)
        //{
            //close(sfd);
            //char arg1[5], arg2[5];
            //int newstdin = dup(stdin), newstdout = dup(stdout);
            //sprintf(arg1, "%d", newstdin);
            //sprintf(arg2, "%d", newstdout);
            //char *agrs[] = {"./client1", arg1, arg2, NULL};
            //execvp(args[0], args);
            char buff[100];
            read(nsfd, buff, sizeof(buff));
            printf("Server Read %s\n", sizeof(buff));
            strcpy(buff, "Hi!");
            write(nsfd, buff, sizeof(buff));
            //close(nsfd);
        /*} else {
            close(nsfd);
        }*/
    }

}
