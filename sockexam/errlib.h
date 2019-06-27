/******************************************************************************
 * errlib.h                                                                   *
 * Library of error functions                                                 *
 * Matteo Corain - Distributed programming I - A.Y. 2018-19                   *
 ******************************************************************************
 * Adapted from Stevens, Unix network programming (3ed).                      *
 * Modifications on the original library include the following:               *
 * - Rewrite of the format of the messages in order to display the PID of the *
 *   calling process.                                                         *
 * - Introduction of the info_msg function, to be used for logging purposes.  *
 ******************************************************************************/

#ifndef     _ERRLIB_H
#define     _ERRLIB_H

#include    <errno.h>
#include    <stdarg.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>

void        err_msg     (const char *fmt, ...);
void        err_quit    (const char *fmt, ...);
void        err_ret     (const char *fmt, ...);
void        err_sys     (const char *fmt, ...);
void        info_msg    (const char *fmt, ...);

#endif
