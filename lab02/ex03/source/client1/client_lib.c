/******************************************************************************
 * client_lib.c : functions for the management of the client                  *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include "client_lib.h"

void addr_setup(const char *ip, const char *port, struct sockaddr_in *addr)
{
    struct in_addr ip_struct;

    /* Clear the memory and set address family */
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;

    /* Convert address and set */
    if (ip == NULL)
        addr->sin_addr.s_addr = INADDR_ANY;
    else
    {
        Inet_pton(AF_INET, ip, &ip_struct);
        addr->sin_addr = ip_struct;
    }
    printf("(%s) Info - Address correctly set: %s.\n", prog_name, inet_ntoa(addr->sin_addr));

    /* Convert port and set */
    addr->sin_port = htons(atoi(port));
    printf("(%s) Info - Port correctly set: %hu.\n", prog_name, ntohs(addr->sin_port));
}

void send_request(int sock, const char *filename)
{
    char buffer[MAXLEN];

    /* Prepare message to be sent */
    sprintf(buffer, "GET %s\r\n", filename);

    /* Send the request */
    Sendn(sock, buffer, strlen(buffer) * sizeof(char), 0);

    printf("(%s) Info - Requested file: %s.\n", prog_name, filename);
}

int read_response(int sock, const char *filename)
{
    int n, fd;
    uint32_t len, len_n, mtime, mtime_n, left;
    char buffer[MAXLEN];

    /* Read the first line */
    Readline(sock, buffer, MAXLEN);

    /* Check the first line */
    if (strncmp(buffer, MSG_ERR, strlen(MSG_ERR)) == 0)
    {
        /* There was an error */
        err_msg("(%s) Error - Cannot retrieve file %s.", prog_name, filename);
        return 0;
    }
    else if (strncmp(buffer, MSG_OK, strlen(MSG_OK)) != 0)
    {
        /* Unknown response */
        err_msg("(%s) Error - Unknown response received.", prog_name);
        return 0;
    }

    /* Open the file in write mode */
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0)
    {
        /* File cannot be opened */
        err_msg("(%s) Error - Output file cannot be opened.", prog_name);
        return 0;
    }

    /* Read the length */
    Recvn(sock, &len_n, sizeof(uint32_t), 0);
    len = ntohl(len_n);

    /* Read the file */
    left = len;
    while (left != 0)
    {
        /* Decide how many bytes to read, left or at least MAXLEN */
        n = (left > MAXLEN) ? MAXLEN : left;

        /* Read socket to buffer */
        Recvn(sock, buffer, n, 0);

        /* Write buffer to file */
        Writen(fd, buffer, n);

        /* Update left */
        left = left - n;
    }

    /* Read the mtime */
    Recvn(sock, &mtime_n, sizeof(uint32_t), 0);
    mtime = ntohl(mtime_n);

    printf("(%s) Info - Received file: %s (len: %d, mtime: %d).\n", prog_name, filename, len, mtime);

    /* Close the file */
    Close(fd);

    return 1;
}