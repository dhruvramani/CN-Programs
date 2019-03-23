/* socket programming - simple - client2 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	/* socket */
	int sfd;	/* socket fd */
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	/* connect */
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(9137);
	socket_address.sin_addr.s_addr = INADDR_ANY;
	
	if(connect(sfd, (struct sockaddr*) &socket_address, sizeof(socket_address)) == -1)
	{
		printf("Unable to connect to server!\n");
		return 1;
	}
	
	/* send/recieve */
	char message[100];
	int n = recv(sfd, message, sizeof(message), 0);
	message[n] = '\0';
	printf("Message recieved to client2 is : \"%s\"\n", message);
	
	/* close */
	close(sfd);	/* closing the socket */
	
	return 0;
}

