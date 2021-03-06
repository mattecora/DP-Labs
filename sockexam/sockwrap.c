/******************************************************************************
 * sockwrap.c                                                                 *
 * Library of wrapper and utility socket functions including error management *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#include "errlib.h"
#include "sockwrap.h"

#define WAIT_RD 0
#define WAIT_WR 1

#define MAXLINE 4096

int line_cnt = 0;
char *line_ptr;
char line_buf[MAXLINE];

/* -------- Socket management functions ------------------------------------- */

int Socket(int family, int type, int protocol, int errmode)
{
    int n;

    if ((n = socket(family, type, protocol)) < 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Socket() failed");
            return -1;
        }
        else
            err_sys("Socket() failed");
    }
    return n;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen, int errmode)
{
    if (bind(sockfd, addr, addrlen) != 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Bind() failed");
            return -1;
        }
        else
            err_sys("Bind() failed");
    }
    return 0;
}

int Listen(int sockfd, int backlog, int errmode)
{
    char *ptr;

    if ((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    
    if (listen(sockfd, backlog) < 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Listen() failed");
            return -1;
        }
        else
            err_sys("Listen() failed");
    }
    return 0;
}

int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlenptr, int errmode)
{
    int n;

again:
    if ((n = accept(sockfd, cliaddr, addrlenptr)) < 0)
    {
        if (errno == EINTR ||
            errno == EPROTO || errno == ECONNABORTED ||
            errno == EMFILE || errno == ENFILE ||
            errno == ENOBUFS || errno == ENOMEM)
            goto again;
        else
        {
            if (errmode == ERR_RET)
            {
                err_ret("Accept() failed");
                return -1;
            }
            else
                err_sys("Accept() failed");
        }
    }

    /* For sockets using linebuf reads, prevent usage of old data from previous reads */
    line_cnt = 0;
    line_ptr = line_buf;
    
    return n;
}

int Connect(int sockfd, const struct sockaddr *srvaddr, socklen_t addrlen, int errmode)
{
    if (connect(sockfd, srvaddr, addrlen) != 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Connect() failed");
            return -1;
        }
        else
            err_sys("Connect() failed");
    }
    return 0;
}

int Select(int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout, int errmode)
{
    int n;

again:
    if ((n = select(nfds, readset, writeset, exceptset, timeout)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
        {
            if (errmode == ERR_RET)
            {
                err_ret("Select() failed");
                return -1;
            }
            else
                err_sys("Select() failed");
        }
    }
    return n;
}

int Close(int fd, int errmode)
{
    if (close(fd) != 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Close() failed");
            return -1;
        }
        else
            err_sys("Close() failed");
    }
    return 0;
}

int Shutdown(int fd, int howto, int errmode)
{
    if (shutdown(fd, howto) != 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Shutdown() failed");
            return -1;
        }
        else
            err_sys("Shutdown() failed");
    }
    return 0;
}

/* -------- Internal I/O functions ------------------------------------------ */

int waitfd(int fd, int timeout, int optype)
{
    fd_set fds;
    struct timeval select_timeout;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    select_timeout.tv_usec = 0;
    select_timeout.tv_sec = timeout;

    if (optype == WAIT_RD)
        return (Select(fd + 1, &fds, NULL, NULL, &select_timeout, ERR_RET) > 0);
    else if (optype == WAIT_WR)
        return (Select(fd + 1, NULL, &fds, NULL, &select_timeout, ERR_RET) > 0);
    return -1;
}

ssize_t readcharbuf(int fd, char *ptr, int timeout, int errmode)
{
    if (line_cnt <= 0)
    {
        if ((line_cnt = Read(fd, line_buf, MAXLINE, timeout, errmode)) < 0)
            return -1;
        else if (line_cnt == 0)
            return 0;
        line_ptr = line_buf;
    }
    line_cnt--;
    *ptr = *line_ptr++;
    return 1;
}

ssize_t recvcharbuf(int fd, char *ptr, int flags, int timeout, int errmode)
{
    if (line_cnt <= 0)
    {
        if ((line_cnt = Recv(fd, line_buf, MAXLINE, flags, timeout, errmode)) < 0)
            return -1;
        else if (line_cnt == 0)
            return 0;
        line_ptr = line_buf;
    }
    line_cnt--;
    *ptr = *line_ptr++;
    return 1;
}

/* -------- Read/write functions -------------------------------------------- */

ssize_t Read(int fd, void *bufptr, size_t nbytes, int timeout, int errmode)
{
    ssize_t n;

again:
    if (!waitfd(fd, timeout, WAIT_RD))
    {
        if (errmode == ERR_RET)
        {
            err_msg("Read() failed: Operation timed out");
            return -1;
        }
        else
            err_quit("Read() failed: Operation timed out");
    }

    if ((n = read(fd, bufptr, nbytes)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
        {
            if (errmode == ERR_RET)
            {
                err_ret("Read() failed");
                return -1;
            }
            else
                err_sys("Read() failed");
        }
    }
    else if (n == 0)
    {
        if (errmode == ERR_RET)
        {
            err_msg("Read() failed: Peer closed the connection");
            return 0;
        }
        else
            err_quit("Read() failed: Peer closed the connection");
    }

    return n;
}

ssize_t Write(int fd, const void *bufptr, size_t nbytes, int timeout, int errmode)
{
    ssize_t n;

    if (!waitfd(fd, timeout, WAIT_WR))
    {
        if (errmode == ERR_RET)
        {
            err_msg("Write() failed: Operation timed out");
            return -1;
        }
        else
            err_quit("Write() failed: Operation timed out");
    }

    if ((n = write(fd, bufptr, nbytes)) != nbytes)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Write() failed");
            return -1;
        }
        else
            err_sys("Write() failed");
    }
    return n;
}

ssize_t Recv(int fd, void *bufptr, size_t nbytes, int flags, int timeout, int errmode)
{
    ssize_t n;

again:
    if (!waitfd(fd, timeout, WAIT_RD))
    {
        if (errmode == ERR_RET)
        {
            err_msg("Recv() failed: Operation timed out");
            return -1;
        }
        else
            err_quit("Recv() failed: Operation timed out");
    }

    if ((n = recv(fd, bufptr, nbytes, flags)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
        {
            if (errmode == ERR_RET)
            {
                err_ret("Recv() failed");
                return -1;
            }
            else
                err_sys("Recv() failed");
        }
    }
    else if (n == 0)
    {
        if (errmode == ERR_RET)
        {
            err_msg("Recv() failed: Peer closed the connection");
            return 0;
        }
        else
            err_quit("Recv() failed: Peer closed the connection");
    }

    return n;
}

ssize_t Recvfrom(int fd, void *bufptr, size_t nbytes, int flags, struct sockaddr *sa, socklen_t *salenptr, int timeout, int errmode)
{
    ssize_t n;

again:
    if (!waitfd(fd, timeout, WAIT_RD))
    {
        if (errmode == ERR_RET)
        {
            err_msg("Recvfrom() failed: Operation timed out");
            return -1;
        }
        else
            err_quit("Recvfrom() failed: Operation timed out");
    }

    if ((n = recvfrom(fd, bufptr, nbytes, flags, sa, salenptr)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
        {
            if (errmode == ERR_RET)
            {
                err_ret("Recvfrom() failed");
                return -1;
            }
            else
                err_sys("Recvfrom() failed");
        }
    }
    else if (n == 0)
    {
        if (errmode == ERR_RET)
        {
            err_msg("Recvfrom() failed: Peer closed the connection");
            return 0;
        }
        else
            err_quit("Recvfrom() failed: Peer closed the connection");
    }
    
    return n;
}

ssize_t Send(int fd, const void *bufptr, size_t nbytes, int flags, int timeout, int errmode)
{
    ssize_t n;

    if (!waitfd(fd, timeout, WAIT_WR))
    {
        if (errmode == ERR_RET)
        {
            err_msg("Send() failed: Operation timed out");
            return -1;
        }
        else
            err_quit("Send() failed: Operation timed out");
    }

    if ((n = send(fd, bufptr, nbytes, flags)) != nbytes)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Send() failed");
            return -1;
        }
        else
            err_sys("Send() failed");
    }
    return n;
}

ssize_t Sendto(int fd, const void *bufptr, size_t nbytes, int flags, const struct sockaddr *sa, socklen_t salen, int timeout, int errmode)
{
    ssize_t n;

    if (!waitfd(fd, timeout, WAIT_WR))
    {
        if (errmode == ERR_RET)
        {
            err_msg("Sendto() failed: Operation timed out");
            return -1;
        }
        else
            err_quit("Sendto() failed: Operation timed out");
    }

    if ((n = sendto(fd, bufptr, nbytes, flags, sa, salen)) != nbytes)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Sendto() failed");
            return -1;
        }
        else
            err_sys("Sendto() failed");
    }
    return n;
}

/* -------- Read/write n bytes functions ------------------------------------ */

ssize_t Readn(int fd, void *bufptr, size_t nbytes, int timeout, int errmode)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = bufptr;
    nleft = nbytes;
    while (nleft > 0)
    {
        if ((nread = Read(fd, ptr, nleft, timeout, errmode)) < 0)
        {
            if (errno == EINTR)
            {
                nread = 0;
                continue; /* and call read() again */
            }
            else
                return -1;
        }
        else if (nread == 0)
            break; /* EOF */

        nleft -= nread;
        ptr += nread;
    }
    return nbytes - nleft;
}

ssize_t Writen(int fd, const void *bufptr, size_t nbytes, int timeout, int errmode)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = bufptr;
    nleft = nbytes;
    while (nleft > 0)
    {
        if ((nwritten = Write(fd, ptr, nleft, timeout, errmode)) <= 0)
        {
            if (errno == EINTR)
            {
                nwritten = 0;
                continue; /* and call write() again */
            }
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return nbytes;
}

ssize_t Recvn(int fd, void *bufptr, size_t nbytes, int flags, int timeout, int errmode)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = bufptr;
    nleft = nbytes;
    while (nleft > 0)
    {
        if ((nread = Recv(fd, ptr, nleft, flags, timeout, errmode)) < 0)
        {
            if (errno == EINTR)
            {
                nread = 0;
                continue; /* and call recv() again */
            }
            else
                return -1;
        }
        else if (nread == 0)
            break; /* EOF */

        nleft -= nread;
        ptr += nread;
    }
    return nbytes - nleft;
}

ssize_t Sendn(int fd, const void *bufptr, size_t nbytes, int flags, int timeout, int errmode)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = bufptr;
    nleft = nbytes;
    while (nleft > 0)
    {
        if ((nwritten = Send(fd, ptr, nleft, flags, timeout, errmode)) <= 0)
        {
            if (errno == EINTR)
            {
                nwritten = 0;
                continue; /* and call send() again */
            }
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return nbytes;
}

ssize_t Readline(int fd, void *bufptr, size_t maxlen, int timeout, int errmode)
{
    int n, rc;
    char c, *ptr;

    ptr = bufptr;
    for (n = 1; n < maxlen; n++)
    {
        if ((rc = Read(fd, &c, 1, timeout, errmode)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
                break; /* newline is stored, like fgets() */
        }
        else if (rc == 0)
        {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        }
        else
            return -1; /* Error, errno set by read() */
    }
    *ptr = 0; /* null terminate like fgets() */
    return n;
}

ssize_t Recvline(int fd, void *bufptr, size_t maxlen, int flags, int timeout, int errmode)
{
    int n, rc;
    char c, *ptr;

    ptr = bufptr;
    for (n = 1; n < maxlen; n++)
    {
        if ((rc = Recv(fd, &c, 1, flags, timeout, errmode)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
                break; /* newline is stored, like fgets() */
        }
        else if (rc == 0)
        {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        }
        else
            return -1; /* Error, errno set by read() */
    }
    *ptr = 0; /* null terminate like fgets() */
    return n;
}

ssize_t Readlinebuf(int fd, void *bufptr, size_t maxlen, int timeout, int errmode)
{
    int n, rc;
    char c, *ptr;

    ptr = bufptr;
    for (n = 1; n < maxlen; n++)
    {
        if ((rc = readcharbuf(fd, &c, timeout, errmode)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
                break; /* newline is stored, like fgets() */
        }
        else if (rc == 0)
        {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        }
        else
            return -1; /* Error, errno set by read() */
    }
    *ptr = 0; /* null terminate like fgets() */
    return n;
}

ssize_t Recvlinebuf(int fd, void *bufptr, size_t maxlen, int flags, int timeout, int errmode)
{
    int n, rc;
    char c, *ptr;

    ptr = bufptr;
    for (n = 1; n < maxlen; n++)
    {
        if ((rc = recvcharbuf(fd, &c, 0, timeout, errmode)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
                break; /* newline is stored, like fgets() */
        }
        else if (rc == 0)
        {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        }
        else
            return -1; /* Error, errno set by read() */
    }
    *ptr = 0; /* null terminate like fgets() */
    return n;
}

/* -------- Translation functions ------------------------------------------- */

int Inet_pton(int af, const char *strptr, void *addrptr, int errmode)
{
    int status = inet_pton(af, strptr, addrptr);

    if (status == 0)
    {
        if (errmode == ERR_RET)
            err_msg("Inet_pton() failed: Invalid address");
        else
            err_quit("Inet_pton() failed: Invalid address");
    }
    else if (status < 0)
    {
        if (errmode == ERR_RET)
            err_ret("Inet_pton() failed");
        else
            err_sys("Inet_pton() failed");
    }
    return status;
}

const char* Inet_ntop(int af, const void *addrptr, char *strptr, size_t length, int errmode)
{
    const char *ret = inet_ntop(af, addrptr, strptr, length);

    if (ret == NULL)
    {
        if (errmode == ERR_RET)
            err_ret("Inet_ntop() failed");
        else
            err_sys("Inet_ntop() failed");
    }
    return ret;
}

int Getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrp, int errmode)
{
    if ((getsockname(sockfd, localaddr, addrp)) != 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Getsockname() failed");
            return -1;
        }
        else
            err_sys("Getsockname() failed");
    }
    return 0;
}

/* -------- Socket status functions ------------------------------------------ */

int Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr, int errmode)
{
    if (getpeername(fd, sa, salenptr) < 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Getpeername() failed");
            return -1;
        }
        else
            err_sys("Getpeername() failed");
    }
    return 0;
}

int Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr, int errmode)
{
    if (getsockopt(fd, level, optname, optval, optlenptr) < 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Getsockopt() failed");
            return -1;
        }
        else
            err_sys("Getsockopt() failed");
    }
    return 0;
}

int Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen, int errmode)
{
    if (setsockopt(fd, level, optname, optval, optlen) < 0)
    {
        if (errmode == ERR_RET)
        {
            err_ret("Setsockopt() failed");
            return -1;
        }
        else
            err_sys("Setsockopt() failed");
    }
    return 0;
}

/* -------- Name resolution functions --------------------------------------- */

int Getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res, int errmode)
{
    int err_code = getaddrinfo(node, service, hints, res);
    
    if (err_code != 0)
    {
        if (errmode == ERR_RET)
        {
            err_msg("Getaddrinfo() failed: %s", gai_strerror(err_code));
            return 0;
        }
        else
            err_quit("Getaddrinfo() failed: %s", gai_strerror(err_code));
    }
    return err_code;
}

int Getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags, int errmode)
{
    int err_code = getnameinfo(addr, addrlen, host, hostlen, serv, servlen, flags);
    
    if (err_code != 0)
    {
        if (errmode == ERR_RET)
        {
            err_msg("Getnameinfo() failed: %s", gai_strerror(err_code));
            return 0;
        }
        else
            err_quit("Getnameinfo() failed: %s", gai_strerror(err_code));
    }
    return err_code;
}