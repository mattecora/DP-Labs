/******************************************************************************
 * sockwrap.h                                                                 *
 * Library of wrapper and utility socket functions including error management *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************
 * Adapted from Stevens, Unix network programming (3ed).                      *
 * Modifications on the original library include the following:               *
 * - All functions present an additional parameter errmode, which can be set  *
 *   to change the behavior of the different functions in case of error (fun- *
 *   ction return for ERR_RET, program termination for ERR_QUIT); when called *
 *   with ERR_QUIT, functions behave as in the original library, while in the *
 *   other case they behave as their non-uppercase counterparts, with additi- *
 *   onal error management capabilities.                                      *
 * - I/O functions present an additional parameter timeout, which can be used *
 *   to set a timeout to be used for the execution of the function; this eff- *
 *   ectively masks a call to Select() with appropriate parameters.           *
 ******************************************************************************/

#ifndef     _SOCKWRAP_H
#define     _SOCKWRAP_H

#include    <arpa/inet.h>
#include    <errno.h>
#include    <netdb.h>
#include    <netinet/in.h>
#include    <sys/socket.h>
#include    <sys/select.h>
#include    <sys/types.h>
#include    <unistd.h>

/* -------- Error behaviors ------------------------------------------------- */

#define     ERR_RET     0
#define     ERR_QUIT    1

/* -------- Socket management functions ------------------------------------- */

int         Socket      (int family, int type, int protocol, int errmode);
int         Bind        (int sockfd, const struct sockaddr *addr, socklen_t addrlen, int errmode);
int         Listen      (int sockfd, int backlog, int errmode);
int         Accept      (int sockfd, struct sockaddr *cliaddr, socklen_t *addrlenptr, int errmode);
int         Connect     (int sockfd, const struct sockaddr *srvaddr, socklen_t addrlen, int errmode);
int         Select      (int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout, int errmode);
int         Close       (int sockfd, int errmode);
int         Shutdown    (int sockfd, int howto, int errmode);

/* -------- Read/write functions -------------------------------------------- */

ssize_t     Read        (int fd, void *bufptr, size_t nbytes, int timeout, int errmode);
ssize_t     Write       (int fd, const void *bufptr, size_t nbytes, int timeout, int errmode);
ssize_t     Recv        (int fd, void *bufptr, size_t nbytes, int flags, int timeout, int errmode);
ssize_t     Recvfrom    (int fd, void *bufptr, size_t nbytes, int flags, struct sockaddr *sa, socklen_t *salenptr, int timeout, int errmode);
ssize_t     Send        (int fd, const void *bufptr, size_t nbytes, int flags, int timeout, int errmode);
ssize_t     Sendto      (int fd, const void *bufptr, size_t nbytes, int flags, const struct sockaddr *sa, socklen_t salen, int timeout, int errmode);

/* -------- Read/write n bytes functions ------------------------------------ */

ssize_t     Readn       (int fd, void *bufptr, size_t nbytes, int timeout, int errmode);
ssize_t     Writen      (int fd, const void *bufptr, size_t nbytes, int timeout, int errmode);

ssize_t     Recvn       (int fd, void *bufptr, size_t nbytes, int flags, int timeout, int errmode);
ssize_t     Sendn       (int fd, const void *bufptr, size_t nbytes, int flags, int timeout, int errmode);

ssize_t     Readline    (int fd, void *bufptr, size_t maxlen, int timeout, int errmode);
ssize_t     Recvline    (int fd, void *bufptr, size_t maxlen, int flags, int timeout, int errmode);
ssize_t     Readlinebuf (int fd, void *bufptr, size_t maxlen, int timeout, int errmode);
ssize_t     Recvlinebuf (int fd, void *bufptr, size_t maxlen, int flags, int timeout, int errmode);

/* -------- Translation functions ------------------------------------------- */

int         Inet_pton   (int af, const char *strptr, void *addrptr, int errmode);
const char* Inet_ntop   (int af, const void *addrptr, char *strptr, size_t length, int errmode);

/* -------- Socket status functions ------------------------------------------ */

int         Getsockname (int sockfd, struct sockaddr *localaddr, socklen_t *addrp, int errmode);
int         Getpeername (int fd, struct sockaddr *sa, socklen_t *salenptr, int errmode);
int         Getsockopt  (int fd, int level, int optname, void *optval, socklen_t *optlenptr, int errmode);
int         Setsockopt  (int fd, int level, int optname, const void *optval, socklen_t optlen, int errmode);

/* -------- Name resolution functions --------------------------------------- */

int         Getaddrinfo (const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res, int errmode);
int         Getnameinfo (const struct sockaddr *addr, socklen_t addrlen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags, int errmode);

#endif