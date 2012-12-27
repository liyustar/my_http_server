#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "SocketUtility.h"
#include "HttpUtility.h"

void process_rq(char *rq, int fd);
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

	for (;;) {	// Run forever
		int clntSock = AcceptTCPConnection(servSock);

		// read request
		char request[BUFSIZ];
		FILE *fpin = fdopen(clntSock, "r");
		while( fgets(request, BUFSIZ, fpin) )
		{
			print_ascii(request);
		}
		printf("|EOF|\n");

		fclose(fpin);
	}

	return 0;	// Not Reached
}

void process_rq(char *request, int clntfd)
{
}

void print_ascii(char *str)
{
	int n = 200;
	while(*str++) printf("%d ", *str);
}


