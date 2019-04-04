/******************************************************************************
 * client_lib.h : definition of functions for the management of the client    *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#ifndef CLIENT_LIB
#define CLIENT_LIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../../../libs/errlib.h"
#include "../../../../libs/sockwrap.h"

#define MAXLEN  4096
#define MSG_OK  "+OK\r\n"
#define MSG_ERR "-ERR\r\n"

extern const char *prog_name;

/* 
 * Description: Setups the sockaddr_in with the given parameters.
 * 
 * Parameters:  - the IP address to be configured (char*), or NULL for any
 *              - the port to be configured (char*)
 *              - the sockaddr_in to be configured (struct sockaddr_in*)
 * 
 * Returns:     nothing
 */
void addr_setup(const char *ip, const char *port, struct sockaddr_in *addr);

/* 
 * Description: Sends a request for the given filename to the given socket.
 * 
 * Parameters:  - the connected socket to which the request is sent (int)
 *              - the name of the file to be requested (char*)
 * 
 * Returns:     nothing
 */
void send_request(int sock, const char *filename);

/* 
 * Description: Reads the response to a request from the given socket.
 * 
 * Parameters:  - the connected socket to which the request was sent (int)
 *              - the name of the requested file (char*)
 * 
 * Returns:     0 in case of error, 1 in case of success
 */
int read_response(int sock, const char *filename);

#endif