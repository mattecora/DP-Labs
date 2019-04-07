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
#include <unistd.h>

#include "../errlib.h"
#include "../sockwrap.h"

#define TIMEOUT 15
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
 * Description: Checks the correctness of a given request and possibly returns
 *              the requested filename.
 * 
 * Parameters:  - the content of the request (char*)
 *              - the location where the filename will be stored (char*)
 * 
 * Returns:     0 in case of wrong request, 1 in case of correct request
 */
int parse_request(char *request, char *filename);

/* 
 * Description: Sends a file according to the described protocol.
 * 
 * Parameters:  - the connected socket to which the file should be sent (int)
 *              - the name of the file to be sent (char*)
 * 
 * Returns:     1 in case the file was transfered successfully, 0 otherwise
 */
int send_file(int conn_sock, char *filename);

/* 
 * Description: Sends an error message according to the described protocol.
 * 
 * Parameters:  - the connected socket to which the error should be sent (int)
 * 
 * Returns:     1 in case of success, 0 otherwise
 */
int send_error(int conn_sock);

/* 
 * Description: Handles a request coming from the given connected socket.
 * 
 * Parameters:  - the connected socket from which the request is coming
 * 
 * Returns:     1 in case a request has been handled, 0 otherwise
 */
int handle_request(int conn_sock);

#endif