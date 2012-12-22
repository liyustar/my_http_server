#ifndef SOCKETUTILITY_H
#define SOCKETUTILITY_H

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Handle error with user msg
void DieWithUserMessage(const char *msg, const char *detail);
// Handle error with sys msg
void DieWithSystemMessage(const char *msg);
// Create, bind, and listen a new TCP server socket
int SetupTCPServerSocket(const char *service);
// Accept a new TCP connection on a server socket
int AcceptTCPConnection(int servSock);

enum sizeConstants {
	MAXCLNT = 5,
};

#endif
