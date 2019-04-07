/******************************************************************************
 * server_lib.c : functions for the management of the server                  *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include "server_lib.h"

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

int parse_request(char *request, char *filename)
{
    /* Check that line starts with GET */
    if (strncmp(request, "GET ", strlen("GET ")) != 0)
        return 0;

    /* Advance pointer */
    request += strlen("GET ");

    /* Try to read the requested filename */
    if (sscanf(request, "%s", filename) != 1)
        return 0;

    /* Advance pointer */
    request += strlen(filename);

    /* Check that line is terminated */
    if (strncmp(request, "\r\n", strlen("\r\n")) != 0)
        return 0;

    /* Everything ok */
    return 1;
}

int send_file(int conn_sock, char *filename)
{
    int n, fd;
    uint32_t len, len_n, mtime, mtime_n, left;
    struct stat file_stat;
    char buffer[MAXLEN];

    /* Open the file */
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return 0;

    /* Get file stats */
    if (fstat(fd, &file_stat) != 0)
        return 0;

    len = file_stat.st_size;
    mtime = file_stat.st_mtime;

    /* Send the ok message */
    if (sendn(conn_sock, MSG_OK, strlen(MSG_OK) * sizeof(char), MSG_NOSIGNAL) < 0)
        return 0;
    
    /* Send the length */
    len_n = htonl(len);
    if (sendn(conn_sock, &len_n, sizeof(uint32_t), MSG_NOSIGNAL) < 0)
        return 0;

    /* Send the file */
    left = len;
    while (left != 0)
    {
        /* Decide how many bytes to read, left or at least MAXLEN */
        n = (left > MAXLEN) ? MAXLEN : left;

        /* Read file to buffer */
        if (readn(fd, buffer, MAXLEN) < 0)
            return 0;

        /* Write buffer to socket */
        if (sendn(conn_sock, buffer, n, MSG_NOSIGNAL) < 0)
            return 0;

        /* Update left */
        left = left - n;

        info_msg("Progress: %d of %d bytes sent (%d%%)",
               len - left, len, (len - left) * 100 / len);
    }

    /* Send the mtime */
    mtime_n = htonl(mtime);
    if (sendn(conn_sock, &mtime_n, sizeof(uint32_t), MSG_NOSIGNAL) < 0)
        return 0;

    info_msg("File sent: %s (len: %d, mtime: %d)", filename, len, mtime);

    /* Close the file */
    close(fd);

    return 1;
}

int send_error(int conn_sock)
{
    /* Send error message */
    return (sendn(conn_sock, MSG_ERR, strlen(MSG_ERR) * sizeof(char), MSG_NOSIGNAL) > 0);
}

int handle_request(int conn_sock)
{
    char request[MAXLEN], filename[MAXLEN];
    fd_set readfds;
    struct timeval select_timeout;

    /* Setup structures for select */
    FD_ZERO(&readfds);
    FD_SET(conn_sock, &readfds);
    select_timeout.tv_sec = TIMEOUT;
    select_timeout.tv_usec = 0;

    /* Wait the socket to be ready */
    if (select(conn_sock + 1, &readfds, NULL, NULL, &select_timeout) <= 0)
    {
        /* Client timeout */
        err_msg("Client timed out");
        return 0;
    }

    /* Read client request */
    if (readline(conn_sock, request, MAXLEN) <= 0)
    {
        /* Connection has been closed by the client */
        info_msg("Client has closed the connection");
        return 0;
    }

    /* Check correctness of the request */
    if (parse_request(request, filename) == 0)
    {
        /* Client sent a wrong request */
        err_msg("Invalid request received");
        send_error(conn_sock);
        return 0;
    }

    /* Check that file exists */
    if (access(filename, F_OK) != 0)
    {
        /* File does not exist */
        err_msg("Requested file does not exist");
        send_error(conn_sock);
        return 0;
    }

    /* Send the file */
    if (send_file(conn_sock, filename) == 0)
    {
        /* Error while sending the file */
        err_msg("File transfer was not successful");
        return 0;
    }

    /* Proceed to the next request */
    return 1;
}