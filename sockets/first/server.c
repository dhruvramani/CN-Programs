/* socket programming basic - server */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int sfd;	/* socket fd */
	
	/* socket */
	sfd = socket(AF_INET, SOCK_STREAM, 0);	/* IPv4, 0 : default protcol */
	
	
	/* bind */
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(9137);
	socket_address.sin_addr.s_addr = INADDR_ANY;	/* we can recieve/send packets from any of the  network interface */
	
	int rval = bind(sfd, (struct sockaddr*) &socket_address, sizeof(socket_address));
	if(rval == -1)
	{
		printf("Error occurred in bind!\n");
		return 1;
	}
	
	/* listen */
	listen(sfd, 5);	/* for now we can handle 5 requests from clients */
	
	/* accept */
	int nsfd;	/* new_socket fd, i.e of client */
	nsfd = accept(sfd, NULL, NULL);	/* if we don't care about client's identity, we can out NULL in address */
	
	/* send/recieve */
	char message[100] = "Message from server to client!";
	send(nsfd, &message, sizeof(message), 0);	/* flags, we would see later */
	
	/* close */
	close(sfd);	/* closing server socket */
	
}
