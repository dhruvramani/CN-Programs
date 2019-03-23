#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 6061
#define SPORT 8088

int main()
{
	struct sockaddr_in servaddr, cliaddr;
	int sfd;

	if((sfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		printf("Socket Error\n");
	
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	cliaddr.sin_port = htons(PORT);

	char buffer[1024];
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(SPORT);

	int len,n;
	 
	sendto(sfd,(const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	 
	 while(1)
	 {
	 	n = recvfrom(sfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&servaddr, sizeof(servaddr));
	 	buffer[n]='\0';
	 	printf("Server sent : %s\n",buffer);
	}
}