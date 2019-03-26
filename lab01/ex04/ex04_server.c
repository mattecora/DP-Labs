/******************************************************************************
 * Lab 01 - Exercise 4 (server)                                               *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../libs/sockwrap.h"

#define BUF_LEN 32

int s;
const char *prog_name = "ex04_server";

void sig_handler(int signal)
{
    if (signal == SIGINT)
    {
        /* Close socket, print message and terminate */
        printf("\n(%s) Info - Received termination command.\n", prog_name);
        Close(s);
        printf("(%s) Info - Socket correctly closed.\n", prog_name);
        exit(0);
    }
}

int main(int argc, char const *argv[])
{
    int len;
    char buffer[BUF_LEN];
    struct sockaddr_in addr;
    struct in_addr ip;

    /* Check input parameters */
    if (argc < 2)
    {
        fprintf(stderr, "(%s) Error - Not enough input parameters.\n", prog_name);
        return -1;
    }

    /* Clear the memory and set address family */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    /* Convert port and set */
    addr.sin_port = htons(atoi(argv[1]));
    printf("(%s) Info - Port correctly set: %hu.\n", prog_name, ntohs(addr.sin_port));

    /* Open the socket */
    s = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("(%s) Info - Socket correctly opened.\n", prog_name);

    /* Bind the socket */
    Bind(s, (struct sockaddr*) &addr, sizeof(addr));
    printf("(%s) Info - Socket correctly bound.\n", prog_name);

    /* Setup the SIGINT handler */
    signal(SIGINT, sig_handler);

    while (1)
    {
        /* Receive a datagram */
        Recvfrom(s, buffer, BUF_LEN, 0, (struct sockaddr*) &addr, &len);
        printf("(%s) Info - Datagram received: \"%s\" from %s:%hu.\n", prog_name, buffer, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        /* Send a datagram */
        Sendto(s, buffer, strlen(buffer), 0, (struct sockaddr*) &addr, sizeof(addr));
        printf("(%s) Info - Datagram sent: \"%s\" to %s:%hu.\n", prog_name, buffer, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    }

    /* Close the socket */
    Close(s);
    printf("(%s) Info - Socket correctly closed.\n", prog_name);

    return 0;
}