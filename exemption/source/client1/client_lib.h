/******************************************************************************
 * client_lib.h : definition of functions for the management of the client    *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************/

#ifndef CLIENT_LIB
#define CLIENT_LIB

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
#define BUFSIZE 4096

#define MSG_OK  "+OK\r\n"
#define MSG_ERR "-ERR\r\n"
#define REQ_FMT "GET %s\r\n"

extern const char *prog_name;

/* 
 * Description: Creates a directory tree to store the given filename.
 * 
 * Parameters:  - the name of the file for which dirs should be created (char*)
 * 
 * Returns:     1 in case of success, 0 otherwise
 */
int create_dirs(const char *filename);

/* 
 * Description: Sends a request for the given filename to the given socket.
 * 
 * Parameters:  - the connected socket to which the request is sent (int)
 *              - the name of the file to be requested (char*)
 * 
 * Returns:     1 in case of success, 0 otherwise
 */
int send_request(int sock, const char *filename);

/* 
 * Description: Reads and parses a response from the given socket.
 * 
 * Parameters:  - the connected socket to which the request was sent (int)
 * 
 * Returns:     1 in case of OK response, 0 otherwise
 */
int parse_response(int sock);

/* 
 * Description: Receives the incoming file from the given socket.
 * 
 * Parameters:  - the connected socket to which the request was sent (int)
 *              - the name of the requested file (char*)
 * 
 * Returns:     1 in case of success, 0 otherwise
 */
int recv_file(int sock, const char *filename);

/* 
 * Description: Controls the interaction with a server, given the socket to
 *              which the server is connected and the filename to request.
 * 
 * Parameters:  - the connected socket to which the request is sent (int)
 *              - the name of the file to be requested (char*)
 *              - a flag indicating if the file to request is the last (int)
 * 
 * Returns:     1 in case of success, 0 otherwise
*/
int run_client(int sock, const char *filename, int last);

#endif