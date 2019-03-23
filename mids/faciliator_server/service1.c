#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>

#define MAX_CLIENTS 5
#define PORT 7100 
int sfd;

void *recvmessage(void *p)
{

	int *a = (int *)p;
	int nsfd = *a;
	char *buff;
	
	while(1)
	{
		buff = malloc(100);
		int y = recv(nsfd, buff, 1024, 0);
		printf("Received : %s\n", buff);
		int d = strlen(buff);
		buff = malloc(100);
		sprintf(buff, "%d", d);
		send(nsfd, buff, strlen(buff),0);
	}
}

int handle()
{
	int nsfd = accept(sfd, NULL, NULL);
	printf("Accepted\n");
	pthread_t pid;
	int a[1] = {nsfd};
	pthread_create(pid, NULL, recvmessage, (void *)a);
	signal(SIGUSR1, handle);
}

int main(int argc,char *argv[])
{

	printf("Service 1\n");
	signal(SIGUSR1, handle);
	sfd = atoi(argv[0]);
	while(1);
}
