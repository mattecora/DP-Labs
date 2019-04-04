/******************************************************************************
 * server1_main.c : sequential TCP server for exercise 2.3                    *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../../../libs/errlib.h"
#include "../../../../libs/sockwrap.h"
#include "server_lib.h"

const char* prog_name;

int main(int argc, char const *argv[])
{
    int list_sock, conn_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;

	prog_name = argv[0];

    /* Check input parameters */
    if (argc < 2)
        err_quit("(%s) Error - Not enough input parameters.", prog_name);
    
    /* Parse the server port */
    addr_setup(NULL, argv[1], &server_addr);

    /* Open the socket */
    list_sock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("(%s) Info - Socket correctly opened.\n", prog_name);

    /* Bind the socket */
    Bind(list_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    printf("(%s) Info - Socket correctly bound.\n", prog_name);

    /* Start listening on the socket */
    Listen(list_sock, 1);
    printf("(%s) Info - Socket started listening.\n", prog_name);

    while (1)
    {
        /* Specify the maximum length of the address */
        client_addr_len = sizeof(client_addr);

        /* Accept a connection from a client */
        conn_sock = Accept(list_sock, (struct sockaddr*) &client_addr, &client_addr_len);

        printf("(%s) Info - Connection opened with %s:%hu.\n",
            prog_name, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* Handle requests from the client */
        while (handle_request(conn_sock));

        /* Close the connected socket */
        Close(conn_sock);

        printf("(%s) Info - Connection closed with %s:%hu.\n",
            prog_name, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    return 0;
}