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
    printf("(%s) Info - Address correctly set: %s.\n", prog_name, inet_ntoa(addr->sin_addr));

    /* Convert port and set */
    addr->sin_port = htons(atoi(port));
    printf("(%s) Info - Port correctly set: %hu.\n", prog_name, ntohs(addr->sin_port));
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

void send_file(int conn_sock, char *filename)
{
    int n, fd;
    uint32_t len, len_n, mtime, mtime_n;
    struct stat file_stat;
    char buffer[MAXLEN];

    /* Open the file */
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return;

    /* Get file stats */
    if (fstat(fd, &file_stat) != 0)
        return;
    
    len = file_stat.st_size;
    mtime = file_stat.st_mtime;

    len_n = htonl(len);
    mtime_n = htonl(mtime);

    printf("(%s) Info - Sending file: %s (len: %d, mtime: %d).\n", prog_name, filename, len, mtime);

    /* Send ok message */
    Sendn(conn_sock, MSG_OK, strlen(MSG_OK) * sizeof(char), 0);

    /* Send length */
    Sendn(conn_sock, &len_n, sizeof(uint32_t), 0);

    /* Send file */
    while ((n = Read(fd, buffer, MAXLEN)) > 0)
    {
        /* Send the buffer contents */
        Sendn(conn_sock, buffer, n, 0);

        /* Clear the buffer */
        memset(buffer, 0, MAXLEN);
    }

    /* Send last modification */
    Sendn(conn_sock, &mtime_n, sizeof(uint32_t), 0);

    /* Close the file */
    Close(fd);
}

void send_error(int conn_sock)
{
    /* Send error message */
    Sendn(conn_sock, MSG_ERR, strlen(MSG_ERR) * sizeof(char), 0);
}

int handle_request(int conn_sock)
{
    char request[MAXLEN], filename[MAXLEN];

    /* Read client request */
    if (readline(conn_sock, request, MAXLEN) <= 0)
    {
        /* Connection has been closed by the client */
        return 0;
    }

    /* Check correctness of the request */
    if (!parse_request(request, filename))
    {
        /* Client sent a wrong request */
        send_error(conn_sock);
        return 0;
    }

    /* Check that file exists */
    if (access(filename, F_OK) != 0)
    {
        /* File does not exist */
        send_error(conn_sock);
        return 0;
    }

    /* Everything ok, send the file */
    send_file(conn_sock, filename);

    /* Proceed to the next request */
    return 1;
}