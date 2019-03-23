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

#define BUF_LEN 32
#define TIMEOUT 5

int s;

void sig_handler(int signal)
{
    if (signal == SIGALRM)
    {
        /* Print message, close socket and terminate */
        printf("CLIENT INFO: no response received!\n");
        close(s);
        printf("CLIENT INFO: socket correctly closed!\n");
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
        fprintf(stderr, "CLIENT ERROR: not enough input parameters.\n");
        return -1;
    }

    /* Clear the memory and set address family */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    /* Convert address and set */
    if (inet_pton(AF_INET, argv[1], &ip) <= 0)
    {
        fprintf(stderr, "CLIENT ERROR: not a valid IP address.\n");
        return -1;
    }
    addr.sin_addr = ip;
    printf("CLIENT INFO: address correctly set (%s).\n", inet_ntoa(addr.sin_addr));

    /* Convert port and set */
    if (sscanf(argv[2], "%hu", &port) <= 0)
    {
        fprintf(stderr, "CLIENT ERROR: not a valid port.\n");
        return -1;
    }
    addr.sin_port = htons(port);
    printf("CLIENT INFO: port correctly set (%hu).\n", ntohs(addr.sin_port));

    /* Open the socket */
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == -1)
    {
        fprintf(stderr, "CLIENT ERROR: cannot create socket.\n");
        return -1;
    }
    printf("CLIENT INFO: socket correctly opened.\n");

    /* Send a datagram */
    if (sendto(s, argv[3], strlen(argv[3]), 0, (struct sockaddr*) &addr, sizeof(addr)) == -1)
    {
        fprintf(stderr, "CLIENT ERROR: cannot write on socket.\n");
        close(s);
        return -1;
    }
    printf("CLIENT INFO: datagram sent (\"%s\" to %s:%hu).\n", argv[3], inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    /* Setup the SIGALRM handler */
    signal(SIGALRM, sig_handler);
    alarm(TIMEOUT);

    /* Receive a datagram */
    if (recvfrom(s, buffer, BUF_LEN, 0, (struct sockaddr*) &addr, &len) == -1)
    {
        fprintf(stderr, "CLIENT ERROR: cannot read from socket.\n");
        close(s);
        return -1;
    }
    printf("CLIENT INFO: datagram received (\"%s\" from %s:%hu).\n", buffer, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    /* Close the socket */
    close(s);

    return 0;
}