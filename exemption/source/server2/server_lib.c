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
        Inet_pton(AF_INET, ip, &ip_struct, ERR_QUIT);
        addr->sin_addr = ip_struct;
    }

    /* Convert port and set */
    addr->sin_port = htons(atoi(port));
}

int read_request(int conn_sock, char *buffer)
{
    /* Read client request */
    if (Recvline(conn_sock, buffer, MAXLEN, 0, TIMEOUT, ERR_RET) == 0)
    {
        /* No data, client has closed the connection */
        info_msg("Client has closed the connection");
        return 0;
    }

    return 1;
}

int check_request(char *request, char *filename)
{
    char cmp_buffer[MAXLEN];

    /* Try to read the filename */
    if (sscanf(request, REQ_FMT, filename) != 1)
    {
        /* Filename cannot be read */
        err_msg("Invalid request format");
        return 0;
    }

    /* Check request format (additional spaces, incorrect termination...) */
    sprintf(cmp_buffer, REQ_FMT, filename);
    if (strncmp(request, cmp_buffer, strlen(cmp_buffer)) != 0)
    {
        /* Request does not match requirements */
        err_msg("Invalid request format");
        return 0;
    }
    
    /* Check the file is accessible */
    if (access(filename, F_OK) != 0)
    {
        /* File is not accessible */
        err_msg("Requested file cannot be accessed");
        return 0;
    }
    
    return 1;
}

int send_file(int conn_sock, char *filename)
{
    int n, fd;
    char buffer[MAXLEN], mtime_pretty[MAXLEN];
    off_t len;
    time_t mtime;
    uint32_t len_n, mtime_n, left;
    struct stat file_stat;

    /* Open the file */
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return 0;

    /* Get file stats */
    if (fstat(fd, &file_stat) != 0)
        return 0;

    len = file_stat.st_size;
    mtime = file_stat.st_mtime;

    /* Get mtime string representation */
    strftime(mtime_pretty, MAXLEN, "%c", localtime(&mtime));

    info_msg("File requested: %s (len: %d bytes, mtime: %s)", filename, len, mtime_pretty);

    /* Send the ok message */
    if (Sendn(conn_sock, MSG_OK, strlen(MSG_OK) * sizeof(char), MSG_NOSIGNAL, TIMEOUT, ERR_RET) < 0)
        return 0;

    /* Send the length */
    len_n = htonl(len);
    if (Sendn(conn_sock, &len_n, sizeof(uint32_t), MSG_NOSIGNAL, TIMEOUT, ERR_RET) < 0)
        return 0;

    /* Send the file */
    left = len;
    while (left != 0)
    {
        /* Decide how many bytes to read, left or at most MAXLEN */
        n = (left > MAXLEN) ? MAXLEN : left;

        /* Read file to buffer */
        if (Readn(fd, buffer, MAXLEN, TIMEOUT, ERR_RET) < 0)
            return 0;

        /* Write buffer to socket */
        if (Sendn(conn_sock, buffer, n, MSG_NOSIGNAL, TIMEOUT, ERR_RET) < 0)
            return 0;

        /* Update left */
        left = left - n;

        info_msg("Progress: %d of %d bytes sent (%d%%)",
                 len - left, len, (len - left) * 100 / len);
    }

    /* Send the mtime */
    mtime_n = htonl(mtime);
    if (Sendn(conn_sock, &mtime_n, sizeof(uint32_t), MSG_NOSIGNAL, TIMEOUT, ERR_RET) < 0)
        return 0;

    info_msg("File sent: %s", filename);

    /* Close the file */
    Close(fd, ERR_RET);

    return 1;
}

int send_error(int conn_sock)
{
    /* Send error message */
    return (Sendn(conn_sock, MSG_ERR, strlen(MSG_ERR) * sizeof(char), MSG_NOSIGNAL, TIMEOUT, ERR_RET) > 0);
}

int run_server(int conn_sock)
{
    char buffer[MAXLEN], filename[MAXLEN];

    /* Read incoming request */
    if (read_request(conn_sock, buffer) == 0)
        return 0;

    /* Parse incoming request */
    if (check_request(buffer, filename) == 0)
    {
        send_error(conn_sock);
        return 0;
    }

    /* Send the file */
    if (send_file(conn_sock, filename) == 0)
        return 0;

    /* Proceed to the next request */
    return 1;
}