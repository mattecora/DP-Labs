#include "errlib.h"
#include "sockwrap.h"

extern char *prog_name;

int Socket(int family, int type, int protocol)
{
    int n;
    if ((n = socket(family, type, protocol)) < 0)
        err_sys("(%s) Error - socket() failed", prog_name);
    return n;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (bind(sockfd, addr, addrlen) != 0)
        err_sys("(%s) Error - bind() failed", prog_name);
}

void Listen(int sockfd, int backlog)
{
    char *ptr;
    if ((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    if (listen(sockfd, backlog) < 0)
        err_sys("(%s) Error - listen() failed", prog_name);
}

int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlenptr)
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
            err_sys("(%s) Error - accept() failed", prog_name);
    }
    return n;
}

void Connect(int sockfd, const struct sockaddr *srvaddr, socklen_t addrlen)
{
    if (connect(sockfd, srvaddr, addrlen) != 0)
        err_sys("(%s) Error - connect() failed", prog_name);
}

int Select(int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
    int n;
again:
    if ((n = select(nfds, readset, writeset, exceptset, timeout)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
            err_sys("(%s) Error - select() failed", prog_name);
    }
    return n;
}

void Close(int fd)
{
    if (close(fd) != 0)
        err_sys("(%s) Error - close() failed", prog_name);
}

void Shutdown(int fd, int howto)
{
    if (shutdown(fd, howto) != 0)
        err_sys("(%s) Error - shutdown() failed", prog_name);
}

ssize_t Read(int fd, void *bufptr, size_t nbytes)
{
    ssize_t n;
again:
    if ((n = read(fd, bufptr, nbytes)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
            err_sys("(%s) Error - read() failed", prog_name);
    }
    return n;
}

ssize_t Write(int fd, void *bufptr, size_t nbytes)
{
    if (write(fd, bufptr, nbytes) != nbytes)
        err_sys("(%s) Error - write() failed", prog_name);
    return nbytes;
}

ssize_t Recv(int fd, void *bufptr, size_t nbytes, int flags)
{
    ssize_t n;

    if ((n = recv(fd, bufptr, nbytes, flags)) < 0)
        err_sys("(%s) Error - recv() failed", prog_name);
    return n;
}

ssize_t Recvfrom(int fd, void *bufptr, size_t nbytes, int flags, struct sockaddr *sa, socklen_t *salenptr)
{
    ssize_t n;

    if ((n = recvfrom(fd, bufptr, nbytes, flags, sa, salenptr)) < 0)
        err_sys("(%s) Error - recvfrom() failed", prog_name);
    return n;
}

ssize_t Send(int fd, void *bufptr, size_t nbytes, int flags)
{
    if (send(fd, bufptr, nbytes, flags) != nbytes)
        err_sys("(%s) Error - send() failed", prog_name);
    return nbytes;
}

ssize_t Sendto(int fd, const void *bufptr, size_t nbytes, int flags, const struct sockaddr *sa, socklen_t salen)
{
    if (sendto(fd, bufptr, nbytes, flags, sa, salen) != nbytes)
        err_sys("(%s) Error - sendto() failed", prog_name);
    return nbytes;
}

ssize_t readn(int fd, void *bufptr, size_t nbytes)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = bufptr;
    nleft = nbytes;
    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0)
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

ssize_t Readn(int fd, void *bufptr, size_t nbytes)
{
    ssize_t n;

    if ((n = readn(fd, bufptr, nbytes)) < 0)
        err_sys("(%s) Error - readn() failed", prog_name);
    return n;
}

ssize_t writen(int fd, void *bufptr, size_t nbytes)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = bufptr;
    nleft = nbytes;
    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
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

ssize_t Writen(int fd, void *bufptr, size_t nbytes)
{
    if (writen(fd, bufptr, nbytes) != nbytes)
        err_sys("(%s) Error - writen() failed", prog_name);
    return nbytes;
}

ssize_t recvn(int fd, void *bufptr, size_t nbytes, int flags)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = bufptr;
    nleft = nbytes;
    while (nleft > 0)
    {
        if ((nread = recv(fd, ptr, nleft, flags)) < 0)
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

ssize_t Recvn(int fd, void *bufptr, size_t nbytes, int flags)
{
    ssize_t n;

    if ((n = recvn(fd, bufptr, nbytes, flags)) < 0)
        err_sys("(%s) Error - readn() failed", prog_name);
    return n;
}

ssize_t sendn(int fd, void *bufptr, size_t nbytes, int flags)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = bufptr;
    nleft = nbytes;
    while (nleft > 0)
    {
        if ((nwritten = send(fd, ptr, nleft, flags)) <= 0)
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

ssize_t Sendn(int fd, void *bufptr, size_t nbytes, int flags)
{
    if (sendn(fd, bufptr, nbytes, flags) != nbytes)
        err_sys("(%s) Error - writen() failed", prog_name);
    return nbytes;
}

ssize_t readline(int fd, void *bufptr, size_t maxlen)
{
    int n, rc;
    char c, *ptr;

    ptr = bufptr;
    for (n = 1; n < maxlen; n++)
    {
        if ((rc = read(fd, &c, 1)) == 1)
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

ssize_t Readline(int fd, void *bufptr, size_t maxlen)
{
    ssize_t n;

    if ((n = readline(fd, bufptr, maxlen)) < 0)
        err_sys("(%s) Error - readline() failed", prog_name);
    return n;
}

ssize_t recvline(int fd, void *bufptr, size_t maxlen, int flags)
{
    int n, rc;
    char c, *ptr;

    ptr = bufptr;
    for (n = 1; n < maxlen; n++)
    {
        if ((rc = recv(fd, &c, 1, flags)) == 1)
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

ssize_t Recvline(int fd, void *bufptr, size_t maxlen, int flags)
{
    ssize_t n;

    if ((n = recvline(fd, bufptr, maxlen, flags)) < 0)
        err_sys("(%s) Error - readline() failed", prog_name);
    return n;
}

void Inet_pton(int af, const char *strptr, void *addrptr)
{
    int status = inet_pton(af, strptr, addrptr);
    if (status == 0)
        err_quit("(%s) Error - inet_pton() failed: Invalid address", prog_name);
    if (status < 0)
        err_sys("(%s) Error - inet_pton() failed", prog_name);
}

void Inet_ntop(int af, const void *addrptr, char *strptr, size_t length)
{
    if (inet_ntop(af, addrptr, strptr, length) == NULL)
        err_sys("(%s) Error - inet_ntop() failed", prog_name);
}

void Getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrp)
{
    if ((getsockname(sockfd, localaddr, addrp)) != 0)
        err_quit("(%s) Error - getsockname() failed", prog_name);
}

void Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    if (getpeername(fd, sa, salenptr) < 0)
        err_sys("(%s) Error - getpeername() failed");
}

void Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr)
{
    if (getsockopt(fd, level, optname, optval, optlenptr) < 0)
        err_sys("(%s) Error - getsockopt() failed");
}

void Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    if (setsockopt(fd, level, optname, optval, optlen) < 0)
        err_sys("(%s) Error - setsockopt() failed");
}

void Getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    int err_code;

    err_code = getaddrinfo(node, service, hints, res);
    if (err_code != 0)
        err_quit("(%s) Error - getaddrinfo() failed: %s", prog_name, gai_strerror(err_code));
}
