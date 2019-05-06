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

int send_request(int sock, const char *filename, XDR *xdr_out)
{
    message request_msg;

    /* Prepare message to be sent */
    memset(&request_msg, 0, sizeof(message));
    request_msg.tag = GET;
    request_msg.message_u.filename = filename;

    /* Write on the XDR buffer */
    if (!xdr_message(xdr_out, &request_msg))
    {
        err_msg("Error while writing on output stream");
        return 0;
    }

    info_msg("File requested: %s", filename);
    return 1;
}

int recv_response(int sock, const char *filename, XDR *xdr_in)
{
    int fd;
    char mtime_pretty[MAXLEN];
    off_t len;
    time_t mtime;
    message response;

    /* Use XDR automatic memory allocation */
    memset(&response, 0, sizeof(message));

    /* Obtain the message */
    if (!xdr_message(xdr_in, &response))
    {
        /* Unknown response */
        err_msg("Response received: unknown");
        return 0;
    }

    /* Read the message type */
    if (response.tag != OK)
    {
        /* Error response */
        err_msg("Response received: error");
        return 0;
    }
    info_msg("Response received: ok");

    /* Open the file in write mode */
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        /* File cannot be opened */
        err_msg("Output file cannot be opened");
        return 0;
    }

    /* Obtain the len */
    len = response.message_u.fdata.contents.contents_len;

    /* Write the output file */
    Writen(fd, response.message_u.fdata.contents.contents_val, len);

    /* Free dynamic memory */
    free(response.message_u.fdata.contents.contents_val);

    /* Obtain the mtime */
    mtime = response.message_u.fdata.last_mod_time;
    strftime(mtime_pretty, MAXLEN, "%c", localtime(&mtime));

    info_msg("File transfer completed: %s (len: %d, mtime: %s)", filename, len, mtime_pretty);

    /* Close the file */
    Close(fd);

    return 1;
}

int run_client(int sock, const char *filename)
{
    FILE *fs_in, *fs_out;
    XDR xdr_in, xdr_out;

    /* Create file streams */
    fs_in = fdopen(sock, "r");
    fs_out = fdopen(sock, "w");

    if (fs_in == NULL || fs_out == NULL)
        return 0;

    /* Create XDR streams */
    xdrstdio_create(&xdr_in, fs_in, XDR_DECODE);
    xdrstdio_create(&xdr_out, fs_out, XDR_ENCODE);

    /* Send a request */
    if (send_request(sock, filename, &xdr_out) == 0)
        return 0;
    fflush(fs_out);

    /* Receive a response */
    if (recv_response(sock, filename, &xdr_in) == 0)
        return 0;

    /* Destroy XDR streams */
    xdr_destroy(&xdr_in);
    xdr_destroy(&xdr_out);

    /* Close file streams */
    fclose(fs_in);
    fclose(fs_out);

    return 1;
}