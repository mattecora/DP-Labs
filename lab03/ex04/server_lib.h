/******************************************************************************
 * server_lib.h : definition of functions for the management of the server    *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#ifndef SERVER_LIB
#define SERVER_LIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
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
int read_request(XDR *xdr_in, message *request);
int check_request(message *request);
int send_file(XDR *xdr_out, message *request);
int send_error(XDR *xdr_out);
int run_server(int conn_sock);

#endif