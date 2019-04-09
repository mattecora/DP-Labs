/******************************************************************************
 * Lab 02 - Exercise 1                                                        *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../libs/errlib.h"
#include "../../libs/sockwrap.h"

#define BUF_LEN 32
#define TIMEOUT 3
#define TRIES 5

const char *prog_name = "ex01";

int main(int argc, char const *argv[])
{
    int i, s;
    socklen_t len;
    char buffer[BUF_LEN];
    fd_set readfds;
    struct sockaddr_in addr;
    struct in_addr ip;
    struct timeval timeout;

    /* Check input parameters */
    if (argc < 4)
    {
        fprintf(stderr, "(%s) Error - Not enough input parameters.", prog_name);
        return -1;
    }

    /* Clear the memory and set address family */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    /* Convert address and set */
    Inet_pton(AF_INET, argv[1], &ip);
    addr.sin_addr = ip;
    printf("(%s) Info - Address correctly set: %s.\n", prog_name, inet_ntoa(addr.sin_addr));

    /* Convert port and set */
    addr.sin_port = htons(atoi(argv[2]));
    printf("(%s) Info - Port correctly set: %hu.\n", prog_name, ntohs(addr.sin_port));

    /* Open the socket */
    s = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("(%s) Info - Socket correctly opened.\n", prog_name);

    /* Receive a datagram */
    for (i = 0; i < TRIES; i++)
    {
        /* Send a datagram */
        Sendto(s, argv[3], strlen(argv[3]), 0, (struct sockaddr*) &addr, sizeof(addr));
        printf("(%s) Info - Datagram sent: \"%s\" to %s:%hu.\n", prog_name, argv[3], inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        /* Setup the timeout structure */
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;
        
        /* Setup the fd_set */
        FD_ZERO(&readfds);
        FD_SET(s, &readfds);

        /* Setup initial len value */
        len = sizeof(struct sockaddr_in);

        /* Wait on the socket for reading */
        if (Select(s+1, &readfds, NULL, NULL, &timeout) > 0)
        {
            /* Receive a datagram */
            Recvfrom(s, buffer, BUF_LEN, 0, (struct sockaddr*) &addr, &len);
            printf("(%s) Info - Datagram received: \"%s\" from %s:%hu.\n", prog_name, buffer, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
            break;
        }
        else
        {
            /* No datagram received */
            printf("(%s) Info - No response received.\n", prog_name);
        }
    }

    /* Close the socket */
    Close(s);
    printf("(%s) Info - Socket correctly closed.\n", prog_name);

    return 0;
}