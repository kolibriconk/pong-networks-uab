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

int main(int argc, char **argv) {

	(void) argc; // This is how an unused parameter warning is silenced.
	(void) argv;

	//domain = domini de comunicació (AF_INIET for IPV4)
	//type = tipus de comuniucació (per udp SOCK _DATAGRAM)
	//protocol = 0 = per defecte al sistema

	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock == -1){
		perror("Error while creating a socket");
		return 1;
	}

	//bind();

	for(int i = 0; i < 3; i++){
		//recvfrom();
		//sendto();
	}

	close(sock);

	return 0;
}

