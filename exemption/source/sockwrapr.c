#include "errlib.h"
#include "sockwrap.h"

int SocketR(int family, int type, int protocol)
{
    int n;
    if ((n = socket(family, type, protocol)) < 0)
        err_ret("Socket() failed");
    return n;
}

int BindR(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int n;
    if ((n = bind(sockfd, addr, addrlen)) != 0)
        err_ret("Bind() failed");
    return n;
}

int ListenR(int sockfd, int backlog)
{
    int n;
    char *ptr;
    if ((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    if ((n = listen(sockfd, backlog)) < 0)
        err_ret("Listen() failed");
    return n;
}

int AcceptR(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlenptr)
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
            err_ret("Accept() failed");
    }
    return n;
}

int ConnectR(int sockfd, const struct sockaddr *srvaddr, socklen_t addrlen)
{
    int n;
    if ((n = connect(sockfd, srvaddr, addrlen)) != 0)
        err_ret("Connect() failed");
    return n;
}

int SelectR(int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
    int n;
again:
    if ((n = select(nfds, readset, writeset, exceptset, timeout)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
            err_ret("Select() failed");
    }
    return n;
}

int CloseR(int fd)
{
    int n;
    if ((n = close(fd)) != 0)
        err_ret("Close() failed");
    return n;
}

int ShutdownR(int fd, int howto)
{
    int n;
    if ((n = shutdown(fd, howto)) != 0)
        err_ret("Shutdown() failed");
    return n;
}

ssize_t ReadR(int fd, void *bufptr, size_t nbytes)
{
    ssize_t n;
again:
    if ((n = read(fd, bufptr, nbytes)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
            err_ret("Read() failed");
    }
    return n;
}

ssize_t WriteR(int fd, const void *bufptr, size_t nbytes)
{
    ssize_t n;
    if ((n = write(fd, bufptr, nbytes)) != nbytes)
        err_ret("Write() failed");
    return n;
}

ssize_t RecvR(int fd, void *bufptr, size_t nbytes, int flags)
{
    ssize_t n;
again:
    if ((n = recv(fd, bufptr, nbytes, flags)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
            err_ret("Recv() failed");
    }
    return n;
}

ssize_t RecvfromR(int fd, void *bufptr, size_t nbytes, int flags, struct sockaddr *sa, socklen_t *salenptr)
{
    ssize_t n;
again:
    if ((n = recvfrom(fd, bufptr, nbytes, flags, sa, salenptr)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
            err_ret("Recvfrom() failed");
    }
    return n;
}

ssize_t SendR(int fd, const void *bufptr, size_t nbytes, int flags)
{
    ssize_t n;
    if ((n = send(fd, bufptr, nbytes, flags)) != nbytes)
        err_ret("Send() failed");
    return n;
}

ssize_t SendtoR(int fd, const void *bufptr, size_t nbytes, int flags, const struct sockaddr *sa, socklen_t salen)
{
    ssize_t n;
    if ((n = sendto(fd, bufptr, nbytes, flags, sa, salen)) != nbytes)
        err_ret("Sendto() failed");
    return n;
}

ssize_t ReadnR(int fd, void *bufptr, size_t nbytes)
{
    ssize_t n;
    if ((n = readn(fd, bufptr, nbytes)) < 0)
        err_ret("Readn() failed");
    return n;
}

ssize_t WritenR(int fd, const void *bufptr, size_t nbytes)
{
    ssize_t n;
    if ((n = writen(fd, bufptr, nbytes)) != nbytes)
        err_ret("Writen() failed");
    return n;
}

ssize_t RecvnR(int fd, void *bufptr, size_t nbytes, int flags)
{
    ssize_t n;
    if ((n = recvn(fd, bufptr, nbytes, flags)) < 0)
        err_ret("Recvn() failed");
    return n;
}

ssize_t SendnR(int fd, const void *bufptr, size_t nbytes, int flags)
{
    ssize_t n;
    if ((n = sendn(fd, bufptr, nbytes, flags)) != nbytes)
        err_ret("Sendn() failed");
    return n;
}

ssize_t ReadlineR(int fd, void *bufptr, size_t maxlen)
{
    ssize_t n;
    if ((n = readline(fd, bufptr, maxlen)) < 0)
        err_ret("Readline() failed");
    return n;
}

ssize_t RecvlineR(int fd, void *bufptr, size_t maxlen, int flags)
{
    ssize_t n;
    if ((n = recvline(fd, bufptr, maxlen, flags)) < 0)
        err_ret("Recvline() failed");
    return n;
}

int Inet_ptonR(int af, const char *strptr, void *addrptr)
{
    int status = inet_pton(af, strptr, addrptr);
    if (status == 0)
        err_msg("Inet_pton() failed: Invalid address");
    if (status < 0)
        err_ret("Inet_pton() failed");
    return status;
}

int Inet_ntopR(int af, const void *addrptr, char *strptr, size_t length)
{
    if (inet_ntop(af, addrptr, strptr, length) == NULL)
    {
        err_ret("Inet_ntop() failed");
        return -1;
    }
    return 0;
}

int GetsocknameR(int sockfd, struct sockaddr *localaddr, socklen_t *addrp)
{
    if ((getsockname(sockfd, localaddr, addrp)) != 0)
    {
        err_ret("Getsockname() failed");
        return -1;
    }
    return 0;
}

int GetpeernameR(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    if (getpeername(fd, sa, salenptr) < 0)
    {
        err_ret("Getpeername() failed");
        return -1;
    }
    return 0;
}

int GetsockoptR(int fd, int level, int optname, void *optval, socklen_t *optlenptr)
{
    if (getsockopt(fd, level, optname, optval, optlenptr) < 0)
    {
        err_ret("Getsockopt() failed");
        return -1;
    }
    return 0;
}

int SetsockoptR(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    if (setsockopt(fd, level, optname, optval, optlen) < 0)
    {
        err_ret("Setsockopt() failed");
        return -1;
    }
    return 0;
}

int GetaddrinfoR(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    int err_code = getaddrinfo(node, service, hints, res);
    if (err_code != 0)
        err_msg("Getaddrinfo() failed: %s", gai_strerror(err_code));
    return err_code;
}