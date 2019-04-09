/******************************************************************************
 * Lab 03 - Exercise 2                                                        *
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

#include "../../libs/errlib.h"
#include "../../libs/sockwrap.h"
#include "server_lib.h"

#define BACKLOG 10

int list_sock;
const char* prog_name;

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        /* Close the socket and terminate */
        Close(list_sock);
        info_msg("Server stopped (PID: %d)", getpid());
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
    int i, conn_sock, child_num;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;

	prog_name = argv[0];

    /* Check input parameters */
    if (argc < 3)
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

    /* Setup the handler for SIGINT and SIGCHLD */
    signal(SIGINT, signal_handler);
    signal(SIGCHLD, signal_handler);

    /* Pre-fork child processes */
    child_num = atoi(argv[2]);
    for (i = 0; i < child_num; i++)
    {
        if (fork() == 0)
        {
            info_msg("Forked child with PID %d", getpid());
            break;
        }
    }

    while (1)
    {
        /* Specify the maximum length of the address */
        client_addr_len = sizeof(client_addr);

        info_msg("Process %d waiting for connection", getpid());

        /* Accept a connection from a client */
        conn_sock = Accept(list_sock, (struct sockaddr*) &client_addr, &client_addr_len);

        info_msg("Connection established with %s:%hu (PID: %d)",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), getpid());

        /* Handle requests from the client */
        while (handle_request(conn_sock));

        /* Close the connected socket */
        Close(conn_sock);

        info_msg("Connection closed with %s:%hu (PID: %d)",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), getpid());
    }

    return 0;
}