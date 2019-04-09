/******************************************************************************
 * Lab 03 - Exercise 3                                                        *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <rpc/xdr.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../libs/sockwrap.h"

#define BUF_LEN 100

const char *prog_name = "ex03";

int main(int argc, char const *argv[])
{
    int s;
    uint16_t n1, n2, sum;
    char buffer[BUF_LEN], buf_in[BUF_LEN], buf_out[BUF_LEN];
    struct sockaddr_in addr;
    struct in_addr ip;
    XDR data_out, data_in;

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

    /* Create the XDR streams */
    xdrmem_create(&data_in, buf_in, BUF_LEN, XDR_DECODE);
    xdrmem_create(&data_out, buf_out, BUF_LEN, XDR_ENCODE);

    while (1)
    {
        /* Read input */
        printf("Insert two numbers (or quit): ");
        fgets(buffer, BUF_LEN, stdin);

        /* Quit if command received */
        if (strncmp(buffer, "quit", strlen("quit")) == 0)
        {
            printf("Received quit.\n");
            break;
        }

        /* Check input */
        if (sscanf(buffer, "%hu %hu", &n1, &n2) < 2)
        {
            fprintf(stderr, "Invalid number format.\n");
            continue;
        }

        /* Prepare and send buffer out */
        if (!xdr_uint16_t(&data_out, &n1) || !xdr_uint16_t(&data_out, &n2))
            break;
        Sendn(s, buf_out, xdr_getpos(&data_out), 0);
        
        /* Prepare and receive buffer in */
        Recv(s, buf_in, BUF_LEN, 0);
        if (!xdr_uint16_t(&data_in, &sum))
            break;

        /* Print the answer */
        printf("Answer: %hu\n", sum);
    }

    /* Close the XDR streams */
    xdr_destroy(&data_in);
    xdr_destroy(&data_out);

    /* Close the socket */
    Close(s);

    return 0;
}