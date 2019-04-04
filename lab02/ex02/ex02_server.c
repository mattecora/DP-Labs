/******************************************************************************
 * Lab 02 - Exercise 2 (server)                                               *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../libs/errlib.h"
#include "../../libs/sockwrap.h"

const char *prog_name = "ex02_server";

int main(int argc, char const *argv[])
{
    int s;
    socklen_t len;
    uint32_t data[2];
    struct sockaddr_in addr;

    /* Check input parameters */
    if (argc < 2)
        err_quit("(%s) Error - Not enough input parameters.", prog_name);

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

    while (1)
    {
        /* Setup initial len value */
        len = sizeof(struct sockaddr_in);

        /* Receive a datagram */
        Recvfrom(s, data, 4, 0, (struct sockaddr*) &addr, &len);
        printf("(%s) Info - Datagram received: \"%u\" from %s:%hu.\n", 
            prog_name, ntohl(data[0]), inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        /* Set the current time */
        data[1] = htonl(time(0));

        /* Send a datagram */
        Sendto(s, data, 8, 0, (struct sockaddr*) &addr, sizeof(addr));
        printf("(%s) Info - Datagram sent: \"%u %u\" to %s:%hu.\n", 
            prog_name, ntohl(data[0]), ntohl(data[1]), inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    }

    /* Close the socket */
    Close(s);
    printf("(%s) Info - Socket correctly closed.\n", prog_name);

    return 0;
}
