#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include "SocketUtility.h"

static const int MAXPENDING = 5;	// Maximum outstanding connection requests

// Handle error with user msg
void DieWithUserMessage(const char *msg, const char *detail) {
	fputs(msg, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	exit(1);
}

// Handle error with sys msg
void DieWithSystemMessage(const char *msg) {
	perror(msg);
	exit(1);
}

// Create, bind, and listen a new TCP server socket
int SetupTCPServerSocket(const char *service) {
	// Construct the server address structure
	struct addrinfo addrCriteria;
	memset(&addrCriteria, 0, sizeof(addrCriteria));
	addrCriteria.ai_family = AF_UNSPEC;
	addrCriteria.ai_flags = AI_PASSIVE;
	addrCriteria.ai_socktype = SOCK_STREAM;
	addrCriteria.ai_protocol = IPPROTO_TCP;

	struct addrinfo *servAddr;	// List of server addresses
	int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
	if (rtnVal != 0)
		DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

	int servSock = -1;
	struct addrinfo *addr = servAddr;
	for ( ; addr != NULL; addr = addr->ai_next) {
		// Create a TCP socket
		servSock = socket(addr->ai_family, addr->ai_socktype,
				addr->ai_protocol);
		if (servSock < 0)
			continue;

		// Bind to the local address and set socket to listen
		if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
				(listen(servSock, MAXPENDING) == 0)) {
			break;
		}

		close(servSock);	// Close and try again
		servSock = -1;
	}

	// Free address list allocated by get addrinfo()
	freeaddrinfo(servAddr);

	return servSock;
}

// Accept a new TCP connection on a server socket
int AcceptTCPConnection(int servSock) {
	struct sockaddr_storage clntAddr;
	// Set length of client address structure (in-out parameter)
	socklen_t clntAddrLen = sizeof(clntAddr);

	// Wait for a client to connect
	int clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &clntAddrLen);
	if (clntSock < 0)
		DieWithSystemMessage("accpet() failed");

	// clntSock is connected to a client
	return clntSock;
}
