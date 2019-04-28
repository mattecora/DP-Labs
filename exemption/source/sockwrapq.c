#include "errlib.h"
#include "sockwrap.h"

int SocketQ(int family, int type, int protocol)
{
    int n;
    if ((n = socket(family, type, protocol)) < 0)
        err_sys("Socket() failed");
    return n;
}

void BindQ(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (bind(sockfd, addr, addrlen) != 0)
        err_sys("Bind() failed");
}

void ListenQ(int sockfd, int backlog)
{
    char *ptr;
    if ((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    if (listen(sockfd, backlog) < 0)
        err_sys("Listen() failed");
}

int AcceptQ(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlenptr)
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
            err_sys("Accept() failed");
    }
    return n;
}

void ConnectQ(int sockfd, const struct sockaddr *srvaddr, socklen_t addrlen)
{
    if (connect(sockfd, srvaddr, addrlen) != 0)
        err_sys("Connect() failed");
}

int SelectQ(int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
    int n;
again:
    if ((n = select(nfds, readset, writeset, exceptset, timeout)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
            err_sys("Select() failed");
    }
    return n;
}

void CloseQ(int fd)
{
    if (close(fd) != 0)
        err_sys("Close() failed");
}

void ShutdownQ(int fd, int howto)
{
    if (shutdown(fd, howto) != 0)
        err_sys("Shutdown() failed");
}

ssize_t ReadQ(int fd, void *bufptr, size_t nbytes)
{
    ssize_t n;
again:
    if ((n = read(fd, bufptr, nbytes)) < 0)
    {
        if (errno == EINTR)
            goto again;
        else
            err_sys("Read() failed");
    }
    return n;
}

ssize_t WriteQ(int fd, const void *bufptr, size_t nbytes)
{
    if (write(fd, bufptr, nbytes) != nbytes)
        err_sys("Write() failed");
    return nbytes;
}

ssize_t RecvQ(int fd, void *bufptr, size_t nbytes, int flags)
{
    ssize_t n;

    if ((n = recv(fd, bufptr, nbytes, flags)) < 0)
        err_sys("Recv() failed");
    return n;
}

ssize_t RecvfromQ(int fd, void *bufptr, size_t nbytes, int flags, struct sockaddr *sa, socklen_t *salenptr)
{
    ssize_t n;

    if ((n = recvfrom(fd, bufptr, nbytes, flags, sa, salenptr)) < 0)
        err_sys("Recvfrom() failed");
    return n;
}

ssize_t SendQ(int fd, const void *bufptr, size_t nbytes, int flags)
{
    if (send(fd, bufptr, nbytes, flags) != nbytes)
        err_sys("Send() failed");
    return nbytes;
}

ssize_t SendtoQ(int fd, const void *bufptr, size_t nbytes, int flags, const struct sockaddr *sa, socklen_t salen)
{
    if (sendto(fd, bufptr, nbytes, flags, sa, salen) != nbytes)
        err_sys("Sendto() failed");
    return nbytes;
}

ssize_t ReadnQ(int fd, void *bufptr, size_t nbytes)
{
    ssize_t n;

    if ((n = readn(fd, bufptr, nbytes)) < 0)
        err_sys("Readn() failed");
    return n;
}

ssize_t WritenQ(int fd, const void *bufptr, size_t nbytes)
{
    if (writen(fd, bufptr, nbytes) != nbytes)
        err_sys("Writen() failed");
    return nbytes;
}

ssize_t RecvnQ(int fd, void *bufptr, size_t nbytes, int flags)
{
    ssize_t n;

    if ((n = recvn(fd, bufptr, nbytes, flags)) < 0)
        err_sys("Recvn() failed");
    return n;
}

ssize_t SendnQ(int fd, const void *bufptr, size_t nbytes, int flags)
{
    if (sendn(fd, bufptr, nbytes, flags) != nbytes)
        err_sys("Sendn() failed");
    return nbytes;
}

ssize_t ReadlineQ(int fd, void *bufptr, size_t maxlen)
{
    ssize_t n;

    if ((n = readline(fd, bufptr, maxlen)) < 0)
        err_sys("Readline() failed");
    return n;
}

ssize_t RecvlineQ(int fd, void *bufptr, size_t maxlen, int flags)
{
    ssize_t n;

    if ((n = recvline(fd, bufptr, maxlen, flags)) < 0)
        err_sys("Recvline() failed");
    return n;
}

void Inet_ptonQ(int af, const char *strptr, void *addrptr)
{
    int status = inet_pton(af, strptr, addrptr);
    if (status == 0)
        err_quit("Inet_pton() failed: Invalid address");
    if (status < 0)
        err_sys("Inet_pton() failed");
}

void Inet_ntopQ(int af, const void *addrptr, char *strptr, size_t length)
{
    if (inet_ntop(af, addrptr, strptr, length) == NULL)
        err_sys("Inet_ntop() failed");
}

void GetsocknameQ(int sockfd, struct sockaddr *localaddr, socklen_t *addrp)
{
    if ((getsockname(sockfd, localaddr, addrp)) != 0)
        err_quit("Getsockname() failed");
}

void GetpeernameQ(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    if (getpeername(fd, sa, salenptr) < 0)
        err_sys("Getpeername() failed");
}

void GetsockoptQ(int fd, int level, int optname, void *optval, socklen_t *optlenptr)
{
    if (getsockopt(fd, level, optname, optval, optlenptr) < 0)
        err_sys("Getsockopt() failed");
}

void SetsockoptQ(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    if (setsockopt(fd, level, optname, optval, optlen) < 0)
        err_sys("Setsockopt() failed");
}

void GetaddrinfoQ(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    int err_code;

    err_code = getaddrinfo(node, service, hints, res);
    if (err_code != 0)
        err_quit("Getaddrinfo() failed: %s", gai_strerror(err_code));
}