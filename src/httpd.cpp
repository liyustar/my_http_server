#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
using namespace std;

#include "SocketUtility.h"
#include "HttpUtility.h"

#define MAX_PROCESS 5

void print_ascii(char *str);

void read_til_crnl(FILE *fp)
{
	char buf[BUFSIZ];
	while( fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n") != 0);
}

// usage: ./httpd 80
int main(int argc, char *argv[]) {
	if (argc != 2)
		DieWithUserMessage("Parameter(s)", "<Server Port/Service>");

	int servSock = SetupTCPServerSocket(argv[1]);

	for (int i=0; i<MAX_PROCESS-1; i++) {	// Run forever
		int pid;
		if( (pid = fork()) == -1)
			perror("fork error");
		else if (pid != 0 && i<MAX_PROCESS-2)
			continue;

		while(1) {
			int clntSock = AcceptTCPConnection(servSock);

			// read request
			Http_message request;
			char buf[BUFSIZ];
			FILE *fpin = fdopen(clntSock, "r");
			fgets(buf, BUFSIZ, fpin);
			request.parseStartLine(buf);			// request start line
			while( fgets(buf, BUFSIZ, fpin) )		// request headers
			{
				if(0 == strcmp("\r\n", buf)) break;
				request.parseHeader(buf);
			}
			printf("|EOF|\n");

			// response
			// cout << request.buildMsgHeader() << "|build finish|" << endl;
			process_rq(request, clntSock);

			fclose(fpin);
		}

		exit(0);
	}

	return 0;	// Not Reached
}

void print_ascii(char *str)
{
	int n = 200;
	while(*str++) printf("%d ", *str);
}


