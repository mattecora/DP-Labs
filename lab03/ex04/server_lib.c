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

int read_request(XDR *xdr_in, message *request)
{
    /* Read client request */
    if (!xdr_message(xdr_in, request))
    {
        /* No data, client has closed the connection */
        info_msg("Client has closed the connection");
        return 0;
    }

    return 1;
}

int check_request(message *request)
{
    /* Check that the message is a GET request */
    if (request->tag != GET)
    {
        /* Invalid request */
        err_msg("Invalid request format");
        return 0;
    }

    /* Check the file is accessible */
    if (access(request->message_u.filename, F_OK) != 0)
    {
        /* File is not accessible */
        err_msg("Requested file cannot be accessed");
        return 0;
    }

    return 1;
}

int send_file(XDR *xdr_out, message *request)
{
    int fd;
    char *file_buf, mtime_pretty[MAXLEN];
    off_t len;
    time_t mtime;
    struct stat file_stat;
    message response;

    /* Open the file */
    fd = open(request->message_u.filename, O_RDONLY);
    if (fd < 0)
        return 0;

    /* Get file stats */
    if (fstat(fd, &file_stat) != 0)
        return 0;

    len = file_stat.st_size;
    mtime = file_stat.st_mtime;

    /* Set the OK message */
    memset(&response, 0, sizeof(message));
    response.tag = OK;

    /* Set the file length and the mtime */
    response.message_u.fdata.contents.contents_len = len;
    response.message_u.fdata.last_mod_time = mtime;

    /* Allocate memory for the file */
    file_buf = malloc(len);

    /* Read the file to memory */
    if (readn(fd, file_buf, len) < 0)
        return 0;
    response.message_u.fdata.contents.contents_val = file_buf;

    /* Write to the XDR buffer */
    if (!xdr_message(xdr_out, &response))
        return 0;

    /* Get mtime string representation */
    strftime(mtime_pretty, MAXLEN, "%c", localtime(&mtime));

    info_msg("File sent: %s (len: %d, mtime: %s)", request->message_u.filename, len, mtime_pretty);

    /* Free dynamic memory */
    free(file_buf);
    free(request->message_u.filename);

    /* Close the file */
    close(fd);

    return 1;
}

int send_error(XDR *xdr_out)
{
    message error;

    /* Prepare error message */
    memset(&error, 0, sizeof(message));
    error.tag = ERR;

    /* Write the XDR stream */
    if (!xdr_message(xdr_out, &error))
        return 0;
    
    return 1;
}

int run_server(int conn_sock)
{
    FILE *fs_in, *fs_out;
    XDR xdr_in, xdr_out;
    message request;

    /* Create file streams */
    fs_in = fdopen(conn_sock, "r");
    fs_out = fdopen(conn_sock, "w");

    if (fs_in == NULL || fs_out == NULL)
        return 0;

    /* Create XDR streams */
    xdrstdio_create(&xdr_in, fs_in, XDR_DECODE);
    xdrstdio_create(&xdr_out, fs_out, XDR_ENCODE);

    /* Read incoming request */
    if (read_request(&xdr_in, &request) == 0)
        return 0;

    /* Parse incoming request */
    if (check_request(&request) == 0)
    {
        send_error(&xdr_out);
        fflush(fs_out);
        return 0;
    }

    /* Send the file */
    if (send_file(&xdr_out, &request) == 0)
    {
        /* Error while sending the file */
        err_msg("File transfer was not successful");
        return 0;
    }
    fflush(fs_out);

    /* Destroy XDR streams */
    xdr_destroy(&xdr_in);
    xdr_destroy(&xdr_out);

    /* Close file streams */
    fclose(fs_in);
    fclose(fs_out);

    /* Proceed to the next request */
    return 1;
}