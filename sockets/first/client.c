/* socket programming basic - client */

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
	/* socket */
	int sfd;	/* socket fd */
	sfd = socket(AF_INET, SOCK_STREAM, 0);	/* IPv4, 0 : default protcol */
	
	/* connect */
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(9137);
	socket_address.sin_addr.s_addr = INADDR_ANY;	/* we can recieve/send packets from any of the  network interface */
	
	int rval;
	rval = connect(sfd, (struct sockaddr*) &socket_address, sizeof(socket_address));
	
	if(rval == -1)
	{
		printf("Unable to connect to server!\n");
		return 1;
	}
	
	/* send/recieve */
	char message[100];
	int n = recv(sfd, message, sizeof(message), 0);
	message[n] = '\0';
	printf("Message recieved to client is : \"%s\"", message);
	
	/* close */
	close(sfd);	/* closing the socket */
	
	
	
	
	
}
