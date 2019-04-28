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
#include    <fcntl.h>
#include    <netdb.h>
#include    <netinet/in.h>
#include    <sys/socket.h>
#include    <sys/select.h>
#include    <sys/types.h>
#include    <unistd.h>

/* -------- Socket management functions ------------------------------------- */

int         SocketQ     (int family, int type, int protocol);
void        BindQ       (int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void        ListenQ     (int sockfd, int backlog);
int         AcceptQ     (int sockfd, struct sockaddr *cliaddr, socklen_t *addrlenptr);
void        ConnectQ    (int sockfd, const struct sockaddr *srvaddr, socklen_t addrlen);
int         SelectQ     (int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);
void        CloseQ      (int sockfd);
void        ShutdownQ   (int sockfd, int howto);

int         SocketR     (int family, int type, int protocol);
int         BindR       (int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int         ListenR     (int sockfd, int backlog);
int         AcceptR     (int sockfd, struct sockaddr *cliaddr, socklen_t *addrlenptr);
int         ConnectR    (int sockfd, const struct sockaddr *srvaddr, socklen_t addrlen);
int         SelectR     (int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);
int         CloseR      (int sockfd);
int         ShutdownR   (int sockfd, int howto);

/* -------- Read/write functions -------------------------------------------- */

ssize_t     ReadQ       (int fd, void *bufptr, size_t nbytes);
ssize_t     WriteQ      (int fd, const void *bufptr, size_t nbytes);
ssize_t     RecvQ       (int fd, void *bufptr, size_t nbytes, int flags);
ssize_t     RecvfromQ   (int fd, void *bufptr, size_t nbytes, int flags, struct sockaddr *sa, socklen_t *salenptr);
ssize_t     SendQ       (int fd, const void *bufptr, size_t nbytes, int flags);
ssize_t     SendtoQ     (int fd, const void *bufptr, size_t nbytes, int flags, const struct sockaddr *sa, socklen_t salen);

ssize_t     ReadR       (int fd, void *bufptr, size_t nbytes);
ssize_t     WriteR      (int fd, const void *bufptr, size_t nbytes);
ssize_t     RecvR       (int fd, void *bufptr, size_t nbytes, int flags);
ssize_t     RecvfromR   (int fd, void *bufptr, size_t nbytes, int flags, struct sockaddr *sa, socklen_t *salenptr);
ssize_t     SendR       (int fd, const void *bufptr, size_t nbytes, int flags);
ssize_t     SendtoR     (int fd, const void *bufptr, size_t nbytes, int flags, const struct sockaddr *sa, socklen_t salen);

/* -------- Read/write n bytes functions ------------------------------------ */

ssize_t     readn       (int fd, void *bufptr, size_t nbytes);
ssize_t     ReadnQ      (int fd, void *bufptr, size_t nbytes);
ssize_t     ReadnR      (int fd, void *bufptr, size_t nbytes);
ssize_t     writen      (int fd, const void *bufptr, size_t nbytes);
ssize_t     WritenQ     (int fd, const void *bufptr, size_t nbytes);
ssize_t     WritenR     (int fd, const void *bufptr, size_t nbytes);

ssize_t     recvn       (int fd, void *bufptr, size_t nbytes, int flags);
ssize_t     RecvnQ      (int fd, void *bufptr, size_t nbytes, int flags);
ssize_t     RecvnR      (int fd, void *bufptr, size_t nbytes, int flags);
ssize_t     sendn       (int fd, const void *bufptr, size_t nbytes, int flags);
ssize_t     SendnQ      (int fd, const void *bufptr, size_t nbytes, int flags);
ssize_t     SendnR      (int fd, const void *bufptr, size_t nbytes, int flags);

ssize_t     readline    (int fd, void *bufptr, size_t maxlen);
ssize_t     ReadlineQ   (int fd, void *bufptr, size_t maxlen);
ssize_t     ReadlineR   (int fd, void *bufptr, size_t maxlen);
ssize_t     recvline    (int fd, void *bufptr, size_t maxlen, int flags);
ssize_t     RecvlineQ   (int fd, void *bufptr, size_t maxlen, int flags);
ssize_t     RecvlineR   (int fd, void *bufptr, size_t maxlen, int flags);

/* -------- Translation functions ------------------------------------------- */

void        Inet_ptonQ  (int af, const char *strptr, void *addrptr);
void        Inet_ntopQ  (int af, const void *addrptr, char *strptr, size_t length);
int         Inet_ptonR  (int af, const char *strptr, void *addrptr);
int         Inet_ntopR  (int af, const void *addrptr, char *strptr, size_t length);

/* -------- Socket status functions ------------------------------------------ */

void        GetsocknameQ(int sockfd, struct sockaddr *localaddr, socklen_t *addrp);
void        GetpeernameQ(int fd, struct sockaddr *sa, socklen_t *salenptr);
void        GetsockoptQ (int fd, int level, int optname, void *optval, socklen_t *optlenptr);
void        SetsockoptQ (int fd, int level, int optname, const void *optval, socklen_t optlen);

int         GetsocknameR(int sockfd, struct sockaddr *localaddr, socklen_t *addrp);
int         GetpeernameR(int fd, struct sockaddr *sa, socklen_t *salenptr);
int         GetsockoptR (int fd, int level, int optname, void *optval, socklen_t *optlenptr);
int         SetsockoptR (int fd, int level, int optname, const void *optval, socklen_t optlen);

/* -------- Name resolution functions --------------------------------------- */

void        GetaddrinfoQ(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
int         GetaddrinfoR(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

#endif
