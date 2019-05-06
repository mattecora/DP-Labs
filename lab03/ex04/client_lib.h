/******************************************************************************
 * client_lib.h : definition of functions for the management of the client    *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#ifndef CLIENT_LIB
#define CLIENT_LIB

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../../libs/errlib.h"
#include "../../libs/sockwrap.h"

#include "xdr_types.h"

#define TIMEOUT 15
#define MAXLEN  4096

extern const char *prog_name;

void addr_setup(const char *ip, const char *port, struct sockaddr_in *addr);
int send_request(int sock, const char *filename, XDR *xdr_out);
int recv_response(int sock, const char *filename, XDR *xdr_in);
int run_client(int sock, const char *filename);

#endif