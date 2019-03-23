/* socket programming - simple - server */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	/* socket */
	int sfd;	/* socket fd */
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	/* bind */
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(9137);
	socket_address.sin_addr.s_addr = INADDR_ANY;
	if((bind(sfd, (struct sockaddr*) &socket_address, sizeof(socket_address))) == -1)
	{
		printf("Error occurred in bind!\n");
		return 1;
	}
	
	/* listen */
	listen(sfd, 5);	/* can handle upto 5 clients */
	
	/* accept */
	int nsfd;	/* new_socket fd */
	nsfd = accept(sfd, NULL, NULL);
	
	/* using fork and closinf sfd in child and nsfd in parent */
	if(fork() > 0)	/* parent */
	{
		/* closing the current client */
		close(nsfd);
		wait(NULL);
	}
	else	/* child */
	{
		/* closing the sfd */
		close(sfd);
		
		/* sending some data to current client */
		char message[100];
		strcpy(message, "Message from server to client1");
		send(nsfd, message, sizeof(message), 0);
		
		exit(0);	/* terminate the child, but the parent will continue */
	}
	/* parent continues */
	
	nsfd = accept(sfd, NULL, NULL);	/* accepting from another client */
	
	/* sending some data to new client */
	char message[100];
	strcpy(message, "Message from server to client2");
	send(nsfd, message, sizeof(message), 0);
	
	return 0;
}

