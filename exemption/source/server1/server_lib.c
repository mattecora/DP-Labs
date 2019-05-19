/******************************************************************************
 * server_lib.c : functions for the management of the server                  *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include "server_lib.h"

int recv_request(int conn_sock, char *buffer)
{
    /* Read client request */
    if (Recvlinebuf(conn_sock, buffer, MAXLEN * sizeof(char), 0, TIMEOUT, ERR_RET) == 0)
    {
        /* No data, client has closed the connection */
        info_msg("Client has closed the connection");
        return 0;
    }

    return 1;
}

int check_request(char *request, char *filename)
{
    char cmp_buffer[MAXLEN], abspath[MAXLEN], cwd[MAXLEN];

    /* Try to read the filename */
    if (sscanf(request, REQ_FMTI, filename) != 1)
    {
        /* Filename cannot be read */
        err_msg("Invalid request format");
        return 0;
    }

    /* Check request format (additional spaces, incorrect termination...) */
    sprintf(cmp_buffer, REQ_FMTO, filename);
    if (strncmp(request, cmp_buffer, strlen(cmp_buffer)) != 0)
    {
        /* Request does not match requirements */
        err_msg("Invalid request format");
        return 0;
    }
    
    /* Check the file is accessible */
    if (access(filename, R_OK) != 0)
    {
        /* File is not accessible */
        err_msg("Requested file (%s) cannot be accessed", filename);
        return 0;
    }

    /* Check that file is in server's working directory */
    realpath(filename, abspath);
    getcwd(cwd, MAXLEN);
    
    if (strncmp(abspath, cwd, strlen(cwd)) != 0)
    {
        /* File is not in server's folder */
        err_msg("Requested file (%s) is not in server's folder", filename);
        return 0;
    }
    
    return 1;
}

int send_file(int conn_sock, char *filename)
{
    int n, fd;
    char buffer[BUFSIZE];
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

    info_msg("File requested: %s (len: %d, mtime: %d)", filename, len, mtime);

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
        /* Decide how many bytes to read, left or at most BUFSIZE */
        n = (left > BUFSIZE) ? BUFSIZE : left;

        /* Read file to buffer */
        if (Readn(fd, buffer, n, TIMEOUT, ERR_RET) < 0)
            return 0;

        /* Write buffer to socket */
        if (Sendn(conn_sock, buffer, n, MSG_NOSIGNAL, TIMEOUT, ERR_RET) < 0)
            return 0;

        /* Update left */
        left = left - n;

        info_msg("Progress: %d of %d bytes sent (%d%%)", len - left, len, (len - left) * 100 / len);
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
    if (recv_request(conn_sock, buffer) == 0)
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