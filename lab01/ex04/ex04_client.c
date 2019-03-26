/******************************************************************************
 * Lab 01 - Exercise 4 (client)                                               *
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
#define TIMEOUT 5

int s;
const char *prog_name = "ex04_client";

void sig_handler(int signal)
{
    if (signal == SIGALRM)
    {
        /* Print message, close socket and terminate */
        printf("(%s) Info - No response received.\n", prog_name);
        close(s);
        printf("(%s) Info - Socket correctly closed.\n", prog_name);
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
    if (argc < 4)
    {
        fprintf(stderr, "(%s) Error - Not enough input parameters.\n", prog_name);
        return -1;
    }

    /* Clear the memory and set address family */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    /* Convert address and set */
    Inet_pton(AF_INET, argv[1], &ip);
    addr.sin_addr = ip;
    printf("(%s) Info - address correctly set: %s.\n", prog_name, inet_ntoa(addr.sin_addr));

    /* Convert port and set */
    addr.sin_port = htons(atoi(argv[2]));
    printf("(%s) Info - Port correctly set: %hu.\n", prog_name, ntohs(addr.sin_port));

    /* Open the socket */
    s = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("(%s) Info - Socket correctly opened.\n", prog_name);

    /* Send a datagram */
    Sendto(s, argv[3], strlen(argv[3]), 0, (struct sockaddr*) &addr, sizeof(addr));
    printf("(%s) Info - Datagram sent: \"%s\" to %s:%hu.\n", prog_name, argv[3], inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    /* Setup the SIGALRM handler */
    signal(SIGALRM, sig_handler);
    alarm(TIMEOUT);

    /* Receive a datagram */
    Recvfrom(s, buffer, BUF_LEN, 0, (struct sockaddr*) &addr, &len);
    printf("(%s) Info - Datagram received: \"%s\" from %s:%hu.\n", prog_name, buffer, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    /* Close the socket */
    Close(s);
    printf("(%s) Info - Socket correctly closed.\n", prog_name);

    return 0;
}