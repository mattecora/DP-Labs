/******************************************************************************
 * sockwrap.h                                                                 *
 * Library of wrapper and utility socket functions including error management *
 * Adapted from Stevens, Unix network programming (3ed)                       *
 ******************************************************************************/

#ifndef     _SOCKWRAP_H
#define     _SOCKWRAP_H

#ifndef     __USE_XOPEN2K
#define     __USE_XOPEN2K
#endif

#include    <arpa/inet.h>
#include    <errno.h>
#include    <netdb.h>
#include    <netinet/in.h>
#include    <sys/socket.h>
#include    <sys/select.h>
#include    <sys/types.h>
#include    <unistd.h>

/* -------- Socket management functions ------------------------------------- */

int         Socket      (int family, int type, int protocol);
void        Bind        (int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void        Listen      (int sockfd, int backlog);
int         Accept      (int sockfd, struct sockaddr *cliaddr, socklen_t *addrlenptr);
void        Connect     (int sockfd, const struct sockaddr *srvaddr, socklen_t addrlen);
int         Select      (int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);
void        Close       (int sockfd);
void        Shutdown    (int sockfd, int howto);

/* -------- Read/write functions -------------------------------------------- */

ssize_t     Read        (int fd, void *bufptr, size_t nbytes);
ssize_t     Write       (int fd, const void *bufptr, size_t nbytes);
ssize_t     Recv        (int fd, void *bufptr, size_t nbytes, int flags);
ssize_t     Recvfrom    (int fd, void *bufptr, size_t nbytes, int flags, struct sockaddr *sa, socklen_t *salenptr);
ssize_t     Send        (int fd, const void *bufptr, size_t nbytes, int flags);
ssize_t     Sendto      (int fd, const void *bufptr, size_t nbytes, int flags, const struct sockaddr *sa, socklen_t salen);

/* -------- Read/write n bytes functions ------------------------------------ */

ssize_t     readn       (int fd, void *bufptr, size_t nbytes);
ssize_t     Readn       (int fd, void *bufptr, size_t nbytes);
ssize_t     writen      (int fd, const void *bufptr, size_t nbytes);
ssize_t     Writen      (int fd, const void *bufptr, size_t nbytes);

ssize_t     recvn       (int fd, void *bufptr, size_t nbytes, int flags);
ssize_t     Recvn       (int fd, void *bufptr, size_t nbytes, int flags);
ssize_t     sendn       (int fd, const void *bufptr, size_t nbytes, int flags);
ssize_t     Sendn       (int fd, const void *bufptr, size_t nbytes, int flags);

ssize_t     readline    (int fd, void *bufptr, size_t maxlen);
ssize_t     Readline    (int fd, void *bufptr, size_t maxlen);
ssize_t     recvline    (int fd, void *bufptr, size_t maxlen, int flags);
ssize_t     Recvline    (int fd, void *bufptr, size_t maxlen, int flags);

/* -------- Translation functions ------------------------------------------- */

void        Inet_pton   (int af, const char *strptr, void *addrptr);
void        Inet_ntop   (int af, const void *addrptr, char *strptr, size_t length);

/* -------- Socket status functions ------------------------------------------ */

void        Getsockname (int sockfd, struct sockaddr *localaddr, socklen_t *addrp);
void        Getpeername (int fd, struct sockaddr *sa, socklen_t *salenptr);
void        Getsockopt  (int fd, int level, int optname, void *optval, socklen_t *optlenptr);
void        Setsockopt  (int fd, int level, int optname, const void *optval, socklen_t optlen);

/* -------- Name resolution functions --------------------------------------- */

void        Getaddrinfo (const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

#endif
