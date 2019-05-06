/******************************************************************************
 * Lab 03 - Exercise 4 (server)                                               *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../libs/errlib.h"
#include "../../libs/sockwrap.h"
#include "server_lib.h"

#define BACKLOG 10

int list_sock;
const char* prog_name;

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        /* Close the socket and terminate */
        Close(list_sock);
        info_msg("Server stopped");
        exit(0);
    }
}

int main(int argc, char const *argv[])
{
    int conn_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;

	prog_name = argv[0];

    /* Check input parameters */
    if (argc < 2)
        err_quit("Not enough input parameters");
    
    /* Parse the server port */
    addr_setup(NULL, argv[1], &server_addr);

    /* Open the socket */
    list_sock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* Bind the socket */
    Bind(list_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));

    /* Start listening on the socket */
    Listen(list_sock, BACKLOG);
    info_msg("Server started on port %s", argv[1]);

    /* Setup the handler for SIGINT */
    signal(SIGINT, sigint_handler);

    while (1)
    {
        /* Specify the maximum length of the address */
        client_addr_len = sizeof(client_addr);

        /* Accept a connection from a client */
        conn_sock = Accept(list_sock, (struct sockaddr*) &client_addr, &client_addr_len);

        info_msg("Connection established with %s:%hu",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* Handle requests from the client */
        while (run_server(conn_sock));

        info_msg("Connection closed with %s:%hu",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    return 0;
}