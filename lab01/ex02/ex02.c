/******************************************************************************
 * Lab 01 - Exercise 2                                                        *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../libs/sockwrap.h"

const char *prog_name = "ex02";

int main(int argc, char const *argv[])
{
    int s;
    struct sockaddr_in addr;
    struct in_addr ip;

    /* Check input parameters */
    if (argc < 3)
    {
        fprintf(stderr, "Not enough input parameters.\n");
        return -1;
    }

    /* Clear the memory and set address family */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    /* Convert address and set */
    Inet_pton(AF_INET, argv[1], &ip);
    addr.sin_addr = ip;
    printf("Address correctly set!\n");

    /* Convert port and set */
    addr.sin_port = htons(atoi(argv[2]));
    printf("Port correctly set!\n");

    /* Open the socket */
    s = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("Socket correctly opened!\n");

    /* Connect the socket */
    Connect(s, (struct sockaddr*) &addr, sizeof(addr));
    printf("Socket correctly connected!\n");

    /* Close the socket */
    Close(s);

    return 0;
}