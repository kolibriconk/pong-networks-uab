/**
 * This file implements a "UDP ping server".
 *
 * It basically waits for datagrams to arrive, and for each one received, it responds to the original sender
 * with another datagram that has the same payload as the original datagram. The server must reply to 3
 * datagrams and then quit.
 *
 * Test with:
 *
 * $ netcat -u 127.0.0.1 8080
 * ping
 * ping
 * pong
 * pong
 *
 * (assuming that this program listens at localhost port 8080)
 *
 */

#include <sys/socket.h>	//for socket creation
#include <stdio.h>	//for standard input output
#include <unistd.h>	//for closing socket
#include <string.h>	//for memset
#include <sys/un.h>	//for sockaddr_un
#include <netinet/in.h>	//for sockaddr_in
#include <arpa/inet.h>	//for htons
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>


#define PORT 8080
#define BUFFER_SIZE 500
#define SOCKET_PATH "/socketcomm"
//#define HOST_ADDRESS "127.0.0.1" //which means localhost
int main(int argc, char **argv) {

	(void) argc; // This is how an unused parameter warning is silenced.
	(void) argv;

	//domain = domini de comunicació (AF_INIET for IPV4)
	//type = tipus de comuniucació (per udp SOCK _DATAGRAM)
	//protocol = 0 = per defecte al sistema

	int sock = socket(AF_INET, SOCK_DGRAM, 0); //creating the socket
	int s;
	if(sock == -1){
		perror("Error while creating a socket");
		return 1;
	}

	struct sockaddr_in addr;
	memset((char*)&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1){
		perror("Error while binding socket to a local address");
		close(sock);
		return 1;
	}

	ssize_t rcv;
	char buff[BUFFER_SIZE];
	socklen_t client_addr_len;
	struct sockaddr_storage client_addr;

	for(int i=0; i<3; i++){
		client_addr_len = sizeof(struct sockaddr_storage);
		rcv = recvfrom(sock, buff, BUFFER_SIZE, 0
			, (struct sockaddr *) &client_addr, &client_addr_len);
		if(rcv == -1)
			continue;
		char host[1], service[1];

		s = getnameinfo((struct sockaddr *) &client_addr,
                        client_addr_len, host, 1,
                        service, 1, 1);

//		printf(rcv);

		sendto(sock, buff, rcv, 0, (struct sockaddr *) &client_addr
			,&client_addr_len);

	}

	close(sock);

	return 0;
}

