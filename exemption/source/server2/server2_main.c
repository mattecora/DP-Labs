/******************************************************************************
 * server2_main.c : concurrent TCP server for exercise 3.1                    *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../errlib.h"
#include "../sockwrap.h"
#include "server_lib.h"

#define BACKLOG 16
#define ADDRLEN 64

int list_sock;
const char* prog_name;

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        /* Close the socket and terminate */
        Close(list_sock, ERR_RET);
        info_msg("Server stopped");
        exit(0);
    }
    else if (sig == SIGCHLD)
    {
        /* A child has terminated, collect the return value */
        wait(NULL);
    }
}

int main(int argc, char const *argv[])
{
    int conn_sock;
    char server_addr_str[ADDRLEN], server_serv_str[ADDRLEN];
    char client_addr_str[ADDRLEN], client_serv_str[ADDRLEN];
    socklen_t client_addr_len;
    struct sockaddr_storage client_addr;
    struct addrinfo hints, *server_addrinfo, *ai;

	prog_name = argv[0];

    /* Check input parameters */
    if (argc < 2)
        err_quit("Not enough input parameters");
    
    /* Prepare hints for name resolution */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    /* Obtain name server_addr_str to bind and listen */
    Getaddrinfo(NULL, argv[1], &hints, &server_addrinfo, ERR_QUIT);
    for (ai = server_addrinfo; ai != NULL; ai = ai->ai_next)
    {
        /* Print the obtained server_addr_str */
        Getnameinfo(ai->ai_addr, ai->ai_addrlen,
            server_addr_str, ADDRLEN, server_serv_str, ADDRLEN, NI_NUMERICHOST | NI_NUMERICSERV, ERR_RET);
        info_msg("Attempting to start on [%s]:%s", server_addr_str, server_serv_str);

        /* Open the socket */
        if ((list_sock = Socket(ai->ai_family, SOCK_STREAM, IPPROTO_TCP, ERR_RET)) < 0)
            continue;

        /* bind and listen the socket */
        if (Bind(list_sock, ai->ai_addr, ai->ai_addrlen, ERR_RET) != 0 ||
            Listen(list_sock, BACKLOG, ERR_RET) != 0)
        {
            Close(list_sock, ERR_RET);
            continue;
        }

        info_msg("Server started on [%s]:%s", server_addr_str, server_serv_str);
        break;
    }

    /* Free data structures and check for connection */
    freeaddrinfo(server_addrinfo);
    if (ai == NULL)
        err_quit("Service cannot be bound");

    /* Setup the handler for SIGINT and SIGCHLD */
    signal(SIGINT, signal_handler);
    signal(SIGCHLD, signal_handler);

    while (1)
    {
        /* Specify the maximum length of the address */
        client_addr_len = sizeof(client_addr);

        info_msg("Father process waiting for connection");

        /* Accept a connection from a client */
        conn_sock = Accept(list_sock, (struct sockaddr *)&client_addr, &client_addr_len, ERR_RET);

        /* Fork and handle requests in the child process */
        if (fork() == 0)
        {
            info_msg("Child process forked");

            /* Read client address and port */
            Getnameinfo((struct sockaddr *)&client_addr, client_addr_len,
                        client_addr_str, ADDRLEN, client_serv_str, ADDRLEN, NI_NUMERICHOST | NI_NUMERICSERV, ERR_RET);
            info_msg("Connection established with [%s]:%s", client_addr_str, client_serv_str);

            /* Close the listening socket */
            Close(list_sock, ERR_RET);

            /* Answer to client requests */
            while (run_server(conn_sock));

            /* Close the connected socket */
            Close(conn_sock, ERR_RET);
            info_msg("Connection closed with [%s]:%s", client_addr_str, client_serv_str);

            info_msg("Child process terminated");
            return 0;
        }

        /* Close the connected socket */
        Close(conn_sock, ERR_RET);
    }

    return 0;
}