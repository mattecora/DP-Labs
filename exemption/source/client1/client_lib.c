/******************************************************************************
 * client_lib.c : functions for the management of the client                  *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include "client_lib.h"

#define CHECK_TIMEOUT(sock, mode, op) \
    ({\
        if (select_socket(sock, TIMEOUT, mode) == 0)\
        {\
            err_msg("Socket timed out");\
            return 0;\
        }\
        op;\
    })

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
        Inet_ptonQ(AF_INET, ip, &ip_struct);
        addr->sin_addr = ip_struct;
    }

    /* Convert port and set */
    addr->sin_port = htons(atoi(port));
}

int select_socket(int sock, int timeout, int mode)
{
    fd_set fds;
    struct timeval select_timeout;

    /* Setup structures for select */
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    select_timeout.tv_usec = 0;
    select_timeout.tv_sec = timeout;

    /* Perform a select */
    if (mode == SELECT_RD)
        return (SelectQ(sock + 1, &fds, NULL, NULL, &select_timeout) > 0);
    return (SelectQ(sock + 1, NULL, &fds, NULL, &select_timeout) > 0);
}

int send_request(int sock, const char *filename)
{
    char buffer[MAXLEN];

    /* Prepare message to be sent */
    sprintf(buffer, REQ_FMT, filename);

    /* Send the request */
    CHECK_TIMEOUT(sock, SELECT_WR, SendnQ(sock, buffer, strlen(buffer) * sizeof(char), 0));

    info_msg("File requested: %s", filename);
    return 1;
}

int parse_response(int sock)
{
    char buffer[MAXLEN];

    /* Read the first line with OK format */
    CHECK_TIMEOUT(sock, SELECT_RD, RecvQ(sock, buffer, strlen(MSG_OK), 0));

    /* Check the first line */
    if (strncmp(buffer, MSG_OK, strlen(MSG_OK)) != 0)
    {
        /* Error or unknown response */
        err_msg("Response received: error or unknown");
        return 0;
    }

    /* OK response */
    info_msg("Response received: ok");
    return 1;
}

int recv_file(int sock, const char *filename)
{
    int n, fd;
    char buffer[MAXLEN], mtime_pretty[MAXLEN];
    off_t len;
    time_t mtime;
    uint32_t len_n, mtime_n, left;
    
    /* Open the file in write mode, or create it */
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        /* File cannot be opened */
        err_msg("Output file cannot be opened");
        return 0;
    }

    /* Read the length */
    CHECK_TIMEOUT(sock, SELECT_RD, RecvnQ(sock, &len_n, sizeof(uint32_t), 0));
    len = ntohl(len_n);
    info_msg("Length of the file: %d bytes", len);

    /* Read the file */
    left = len;
    while (left != 0)
    {
        /* Decide how many bytes to read, left or at most MAXLEN */
        n = (left > MAXLEN) ? MAXLEN : left;

        /* Read socket to buffer */
        CHECK_TIMEOUT(sock, SELECT_RD, RecvnQ(sock, buffer, n, 0));

        /* Write buffer to file */
        CHECK_TIMEOUT(fd, SELECT_WR, WritenQ(fd, buffer, n));

        /* Update left */
        left = left - n;

        info_msg("Progress: %d of %d bytes received (%d%%)", len - left, len, (len - left)*100/len);
    }

    /* Read the mtime */
    CHECK_TIMEOUT(sock, SELECT_RD, RecvnQ(sock, &mtime_n, sizeof(uint32_t), 0));
    mtime = ntohl(mtime_n);

    /* Pretty-print the mtime */
    strftime(mtime_pretty, MAXLEN, "%c", localtime(&mtime));
    info_msg("Last modification time of the file: %s", mtime_pretty);

    info_msg("File received: %s (len: %d bytes, mtime: %s)", filename, len, mtime_pretty);

    /* Close the file */
    CloseQ(fd);

    return 1;
}

int run_client(int sock, const char *filename)
{
    /* Send a request for the file */
    send_request(sock, filename);

    /* Check the response */
    if (parse_response(sock) != 0)
    {
        /* Receive the file */
        return recv_file(sock, filename);
    }

    /* An error has occurred */
    return 0;
}