#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUFFER_SIZE 65535
#define PORT "8080"

int main(int argc, char **argv)
{

	(void) argc;
	(void) argv;

	struct addrinfo addr;
	struct addrinfo *result;
	int sock;
	struct sockaddr_storage sock_addr;
	socklen_t sock_addr_len;
	ssize_t rcv;
	char buff[BUFFER_SIZE];

	memset(&addr, 0, sizeof(struct addrinfo));
	addr.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
	addr.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	addr.ai_protocol = 0;          /* Any protocol */

	if (getaddrinfo(NULL, PORT, &addr, &result) != 0) {
		printf("Error getting address of host");
		return 1;
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == -1){
		printf("Error creating socket");
		return 1;
	}

	if (bind(sock, (*result).ai_addr, (*result).ai_addrlen) == -1){
		printf("Error binding port");
		return 1;
	}

	for (int i = 0; i < 3; i++) {
		sock_addr_len = sizeof(struct sockaddr_storage);
		rcv = recvfrom(sock, buff, BUFFER_SIZE, 0,
			(struct sockaddr *) &sock_addr, &sock_addr_len);

		if (sendto(sock, buff, rcv, 0, (struct sockaddr *) &sock_addr,
			    sock_addr_len) != rcv)
		    fprintf(stderr, "Error sending pong");
	}
}
