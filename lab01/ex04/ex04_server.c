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

#define BUF_LEN 32

int s;

void sig_handler(int signal)
{
    if (signal == SIGINT)
    {
        /* Close socket, print message and terminate */
        printf("\nSERVER INFO: received termination command.\n");
        close(s);
        printf("SERVER INFO: socket correctly closed.\n");
        exit(0);
    }
}

int main(int argc, char const *argv[])
{
    int len;
    uint16_t port;
    char buffer[BUF_LEN];
    struct sockaddr_in addr;
    struct in_addr ip;

    /* Check input parameters */
    if (argc < 2)
    {
        fprintf(stderr, "SERVER ERROR: not enough input parameters.\n");
        return -1;
    }

    /* Clear the memory and set address family */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    /* Convert port and set */
    if (sscanf(argv[1], "%hu", &port) <= 0)
    {
        fprintf(stderr, "SERVER ERROR: not a valid port.\n");
        return -1;
    }
    addr.sin_port = htons(port);
    printf("SERVER INFO: port correctly set (%hu).\n", ntohs(addr.sin_port));

    /* Open the socket */
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == -1)
    {
        fprintf(stderr, "SERVER ERROR: cannot create socket.\n");
        return -1;
    }
    printf("SERVER INFO: socket correctly opened.\n");

    /* Bind the socket */
    if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) == -1)
    {
        fprintf(stderr, "SERVER ERROR: cannot bind socket.\n");
        close(s);
        return -1;
    }
    printf("SERVER INFO: socket correctly bound.\n");

    /* Setup the SIGINT handler */
    signal(SIGINT, sig_handler);

    while (1)
    {
        /* Receive a datagram */
        if (recvfrom(s, buffer, BUF_LEN, 0, (struct sockaddr*) &addr, &len) == -1)
        {
            fprintf(stderr, "SERVER ERROR: cannot read from socket.\n");
            close(s);
            return -1;
        }
        printf("SERVER INFO: datagram received (\"%s\" from %s:%hu).\n", buffer, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        /* Send a datagram */
        if (sendto(s, buffer, strlen(buffer), 0, (struct sockaddr*) &addr, sizeof(addr)) == -1)
        {
            fprintf(stderr, "SERVER ERROR: cannot write on socket.\n");
            close(s);
            return -1;
        }
        printf("SERVER INFO: datagram sent (\"%s\" to %s:%hu).\n", buffer, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    }

    /* Close the socket */
    close(s);
    printf("SERVER INFO: socket correctly closed.\n");

    return 0;
}