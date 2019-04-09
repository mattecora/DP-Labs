/******************************************************************************
 * client1_main.c : TCP client for exercise 2.3                               *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../errlib.h"
#include "../sockwrap.h"
#include "client_lib.h"

const char* prog_name;

int main(int argc, char const *argv[])
{
	int i, sock;
	struct sockaddr_in server_addr;

	prog_name = argv[0];

	/* Check number of parameters */
    if (argc < 4)
		err_quit("Not enough input parameters");
	
	/* Parse the server address and port */
    addr_setup(argv[1], argv[2], &server_addr);

	/* Open the socket */
    sock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/* Connect the socket */
	Connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
	
	info_msg("Connection established with %s:%s", argv[1], argv[2]);

	/* Loop through the arguments */
	for (i = 3; i < argc; i++)
	{
		/* Send a request for the file */
		send_request(sock, argv[i]);

		/* Receive the response */
		if (recv_response(sock, argv[i]) == 0)
		{
			/* There was an error, close the connection */
			break;
		}
	}

	/* Close the socket */
	Close(sock);

	info_msg("Connection closed with %s:%s", argv[1], argv[2]);

    return 0;
}