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

int main(int argc, char const *argv[])
{
    int s;
    uint16_t port;
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
    if (inet_pton(AF_INET, argv[1], &ip) <= 0)
    {
        fprintf(stderr, "Not a valid IP address!\n");
        return -1;
    }
    addr.sin_addr = ip;
    printf("Address correctly set!\n");

    /* Convert port and set */
    if (sscanf(argv[2], "%hu", &port) <= 0)
    {
        fprintf(stderr, "Not a valid port!\n");
        return -1;
    }
    addr.sin_port = htons(port);
    printf("Port correctly set!\n");

    /* Open the socket */
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == -1)
    {
        fprintf(stderr, "Cannot create socket.\n");
        return -1;
    }
    printf("Socket correctly opened!\n");

    /* Connect the socket */
    if (connect(s, (struct sockaddr*) &addr, sizeof(addr)) == -1)
    {
        fprintf(stderr, "Cannot connect socket.\n");
        return -1;
    }
    printf("Socket correctly connected!\n");

    /* Close the socket */
    close(s);

    return 0;
}