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

    /* Convert port and set */
    addr->sin_port = htons(atoi(port));
}

void send_request(int sock, const char *filename)
{
    char buffer[MAXLEN];

    /* Prepare message to be sent */
    sprintf(buffer, "GET %s\r\n", filename);

    /* Send the request */
    Sendn(sock, buffer, strlen(buffer) * sizeof(char), 0);

    info_msg("File requested: %s", filename);
}

int recv_response(int sock, const char *filename)
{
    int n, fd;
    char buffer[MAXLEN];
    uint32_t len, len_n, mtime, mtime_n, left;
    fd_set readfds;
    struct timeval select_timeout;

    /* Setup structures for select */
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    select_timeout.tv_usec = 0;

    /* Wait the socket to be ready */
    select_timeout.tv_sec = TIMEOUT;
    if (Select(sock + 1, &readfds, NULL, NULL, &select_timeout) <= 0)
    {
        err_msg("Server timed out");
        return 0;
    }

    /* Read the first line */
    Readline(sock, buffer, MAXLEN);

    /* Check the first line */
    if (strncmp(buffer, MSG_ERR, strlen(MSG_ERR)) == 0)
    {
        /* Error response */
        err_msg("Response received: error");
        return 0;
    }
    else if (strncmp(buffer, MSG_OK, strlen(MSG_OK)) != 0)
    {
        /* Unknown response */
        err_msg("Response received: unknown");
        return 0;
    }
    info_msg("Response received: ok");

    /* Open the file in write mode */
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0)
    {
        /* File cannot be opened */
        err_msg("Output file cannot be opened");
        return 0;
    }

    /* Wait the socket to be ready */
    select_timeout.tv_sec = TIMEOUT;
    if (Select(sock + 1, &readfds, NULL, NULL, &select_timeout) <= 0)
    {
        err_msg("Server timed out");
        return 0;
    }

    /* Read the length */
    Recvn(sock, &len_n, sizeof(uint32_t), 0);
    len = ntohl(len_n);
    info_msg("Length of the file: %d bytes", len);

    /* Read the file */
    left = len;
    while (left != 0)
    {
        /* Decide how many bytes to read, left or at least MAXLEN */
        n = (left > MAXLEN) ? MAXLEN : left;

        /* Wait the socket to be ready */
        select_timeout.tv_sec = TIMEOUT;
        if (Select(sock + 1, &readfds, NULL, NULL, &select_timeout) <= 0)
        {
            err_msg("Server timed out");
            return 0;
        }

        /* Read socket to buffer */
        Recvn(sock, buffer, n, 0);

        /* Write buffer to file */
        Writen(fd, buffer, n);

        /* Update left */
        left = left - n;

        info_msg("Progress: %d of %d bytes received (%d%%)", len - left, len, (len - left)*100/len);
    }

    /* Wait the socket to be ready */
    select_timeout.tv_sec = TIMEOUT;
    if (Select(sock + 1, &readfds, NULL, NULL, &select_timeout) <= 0)
    {
        err_msg("Server timed out");
        return 0;
    }

    /* Read the mtime */
    Recvn(sock, &mtime_n, sizeof(uint32_t), 0);
    mtime = ntohl(mtime_n);
    info_msg("Last modification time of the file: %d", mtime);

    info_msg("File transfer completed: %s (len: %d, mtime: %d)", filename, len, mtime);

    /* Close the file */
    Close(fd);

    return 1;
}