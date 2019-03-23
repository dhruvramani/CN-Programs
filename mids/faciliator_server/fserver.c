#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/select.h>
#include<signal.h>
#include<pthread.h>

#define PORT 8088
#define limit 25
#define MAX  25

char *path[MAX];
int sfd, count = 1, clicount = 0, pids[MAX], fds[MAX]];
struct sockaddr_in cliaddr[5 * MAX];

void *send_all(void *p)
{
	int len;
	char *buff;
	while(1)
	{
			buff = malloc(100);
			struct sockaddr_in caddr;
			int n = recvfrom(sfd, (char *)buff, 1024, 0, (struct sockaddr *)&caddr, &len);
			
			cliaddr[clicount] = caddr; 
			clicount++;
			socklen_t d = (socklen_t) sizeof(caddr);
			
			for(int i=0;i<count-1;i++) 
			{
				printf("Sending to client %d\n",i);
				buff = malloc(100);
				strcpy(buff, path[i]);
				int w = sendto(sfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *)&caddr, d);
			}
	}
}

int info()
{

		char *buff;
		buff = malloc(100);
		strcpy(buff, path[count - 2]);
		for(int i = 0;i < clicount; i++)
		{
			socklen_t d = (socklen_t)sizeof(cliaddr[i]);
			int w = sendto(sfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *)&cliaddr[i], d);
		}
}

int get_port(char *s)
{
	int x=0;
	for(int i = 0; s[i] && s[i] >= '0' && s[i] <= '9'; i++)
		x = x * 10 + s[i] - '0';
	return x;
}

int main()
{

	signal(SIGUSR1,info);	
	
	if((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		printf("Socket Error\n");

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if(bind(sfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Bind Error\n");
		return 1;
	}	

	pthread_t tid;
	pthread_create(&tid, NULL, send_all, NULL);

	char buff[1024], *s;
	fds[0] = 0;
	
	fd_set rset;
	FD_SET(fds[0], &rset);
	int maxfd = 0;

	while(1)
	{

		for(int i = 0;i < count; i++)
			FD_SET(fds[i], &rset);

		select(maxfd + 1, &rset, NULL, NULL, NULL);

		for(int i = 1;i < count; i++)
		{
			if(FD_ISSET(fds[i],&rset))
			{
				printf("Sending signal to process\n");
				kill(pids[i-1],SIGUSR1);
			}
		}

		if(FD_ISSET(fds[0], &rset)) 
		{
			s = malloc(100);
			int y = read(fds[0], s, 1024);
			
			int p = get_port(s);
			char *s1 = malloc(100);
			sprintf(s1, "%d", p);
			s = s +  strlen(s1) + 1;

			printf("Port %d\n", p);
			path[count-1] = malloc(100);
			strcpy(path[count-1], s);

			struct sockaddr_in address;
   			address.sin_family = AF_INET;
    		address.sin_addr.s_addr = inet_addr("127.0.0.1");
		    address.sin_port = htons(p);
			int addrlen = sizeof(address);
	
			fds[count] = socket(AF_INET, SOCK_STREAM, 0);
			
			if(maxfd < fds[count])
				maxfd = fds[count];

			bind(fds[count], (struct sockaddr *)&address, addrlen);
			listen(fds[count], MAX);
			count++;

			int c = fork();
			if(c > 0)
				pids[count - 2] = c;
			else
			{   char *news = malloc(100);
				strncpy(news, s, strlen(s) - 1);
				s = malloc(100);
				sprintf(s, "%d", fds[count-1]);
				int x = execlp(news, s, NULL);
				break;
			}

			kill(getpid(), SIGUSR1);
		}
	}

	return 0;
}