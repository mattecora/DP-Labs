#include "errlib.h"
#include "sockwrap.h"

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

ssize_t writen(int fd, const void *bufptr, size_t nbytes)
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

ssize_t sendn(int fd, const void *bufptr, size_t nbytes, int flags)
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