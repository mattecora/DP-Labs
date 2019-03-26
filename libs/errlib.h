/******************************************************************************
 * errlib.h                                                                   *
 * Library of error functions                                                 *
 * Adapted from Stevens, Unix network programming (3ed)                       *
 ******************************************************************************/

#ifndef     _ERRLIB_H
#define     _ERRLIB_H

#include    <errno.h>
#include    <stdarg.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <syslog.h>

extern int  daemon_proc;

void        err_msg     (const char *fmt, ...);
void        err_quit    (const char *fmt, ...);
void        err_ret     (const char *fmt, ...);
void        err_sys     (const char *fmt, ...);
void        err_dump    (const char *fmt, ...);

#endif
