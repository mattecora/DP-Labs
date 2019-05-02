/******************************************************************************
 * server_lib.h : definition of functions for the management of the server    *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#ifndef SERVER_LIB
#define SERVER_LIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
#define REQ_FMT "GET %s\r\n"

#define SELECT_RD 0
#define SELECT_WR 1

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
 * Description: Performs a select on the given socket waiting for it to be
 *              ready for reading or writing.
 * 
 * Parameters:  - the socket to wait for (int)
 *              - the maximum timeout before aborting (int)
 *              - the mode of the operation (O_RD or O_WR, int)
 * 
 * Returns:     1 in case the socket was ready before timeout, 0 otherwise
 */
int select_socket(int sock, int timeout, int mode);

/* 
 * Description: Reads a request from the given socket in the given buffer.
 * 
 * Parameters:  - the connected socket from which the request is coming (int)
 *              - the location where the request will be stored (char*)
 * 
 * Returns:     0 in case of wrong request, 1 in case of correct request
 */
int read_request(int conn_sock, char *buffer);

/* 
 * Description: Checks the correctness of a given request and possibly returns
 *              the requested filename.
 * 
 * Parameters:  - the content of the request (char*)
 *              - the location where the filename will be stored (char*)
 * 
 * Returns:     0 in case of wrong request, 1 in case of correct request
 */
int check_request(char *request, char *filename);

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
 * Description: Controls the interaction with a client, given the socket to
 *              which the client is connected.
 * 
 * Parameters:  - the connected socket of the client to interact with (int)
 * 
 * Returns:     1 in case a request has been handled, 0 otherwise
 */
int run_server(int conn_sock);

#endif