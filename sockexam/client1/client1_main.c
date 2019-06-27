/******************************************************************************
 * client1_main.c                                                             *
 * TCP client for exercise 2.3                                                *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************
 * NOTE: This program uses a modified version of the sockwrap library, as de- *
 *       scribed in sockwrap.h.                                               *
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

#define ADDRLEN 64

const char* prog_name;

int main(int argc, char const *argv[])
{
    int i, sock;
    char server_addr_str[ADDRLEN], server_serv_str[ADDRLEN];
    struct addrinfo hints, *server_addrinfo, *ai;

    prog_name = argv[0];

    /* Check number of parameters */
    if (argc < 4)
        err_quit("Not enough input parameters");

    /* Prepare hints for name resolution */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    /* Resolve server name */
    Getaddrinfo(argv[1], argv[2], &hints, &server_addrinfo, ERR_QUIT);
    for (ai = server_addrinfo; ai != NULL; ai = ai->ai_next)
    {
        /* Print the obtained address */
        Getnameinfo(ai->ai_addr, ai->ai_addrlen,
            server_addr_str, ADDRLEN, server_serv_str, ADDRLEN, NI_NUMERICHOST | NI_NUMERICSERV, ERR_QUIT);
        info_msg("Attempting to connect to [%s]:%s", server_addr_str, argv[2]);
        
        /* Open the socket */
        if ((sock = Socket(ai->ai_family, SOCK_STREAM, IPPROTO_TCP, ERR_RET)) < 0)
            continue;
        
        /* Connect the socket */
        if (Connect(sock, ai->ai_addr, ai->ai_addrlen, ERR_RET) != 0)
        {
            Close(sock, ERR_RET);
            continue;
        }

        info_msg("Connection established with [%s]:%s", server_addr_str, argv[2]);
        break;
    }

    /* Free data structures and check for connection */
    freeaddrinfo(server_addrinfo);
    if (ai == NULL)
        err_quit("Server name cannot be resolved");

    /* Loop through the arguments */
    for (i = 3; i < argc; i++)
    {
        /* Handle a request and, in case of error, break */
        if (run_client(sock, argv[i], i == (argc - 1)) == 0)
            break;
    }

    /* Close the socket */
    Close(sock, ERR_QUIT);
    info_msg("Connection closed with [%s]:%s", server_addr_str, argv[2]);

    return 0;
}