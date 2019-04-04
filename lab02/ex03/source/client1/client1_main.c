/******************************************************************************
 * Lab 02 - Exercise 3 (client)                                               *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../../../libs/errlib.h"
#include "../../../../libs/sockwrap.h"
#include "client_lib.h"

const char *prog_name;

int main(int argc, char const *argv[])
{
	int i, sock;
	struct sockaddr_in server_addr;

	prog_name = argv[0];

	/* Check number of parameters */
    if (argc < 4)
		err_quit("(%s) Error - Not enough input parameters.", prog_name);
	
	/* Parse the server address and port */
    addr_setup(argv[1], argv[2], &server_addr);

	/* Open the socket */
    sock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("(%s) Info - Socket correctly opened.\n", prog_name);

	/* Connect the socket */
	Connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
	printf("(%s) Info - Socket correctly connected.\n", prog_name);

	/* Loop through the arguments */
	for (i = 3; i < argc; i++)
	{
		/* Send a request for the file */
		send_request(sock, argv[i]);

		/* Read the response */
		if (read_response(sock, argv[i]) == 0)
		{
			/* There was an error, close the connection */
			break;
		}
	}

	/* Close the socket */
	Close(sock);

    return 0;
}