#include "errlib.h"

#define MAXLINE 4095

extern char* prog_name;

/* Nonfatal error related to system call
 * Print message and return */

void err_ret(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "(%s) Error - ", prog_name);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ": %s.\n", strerror(errno));
    va_end(ap);
    return;
}

/* Fatal error related to system call
 * Print message and terminate */

void err_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "(%s) Error - ", prog_name);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ": %s.\n", strerror(errno));
    exit(1);
}

/* Nonfatal error unrelated to system call
 * Print message and return */

void err_msg(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "(%s) Error - ", prog_name);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
    va_end(ap);
    return;
}

/* Fatal error unrelated to system call
 * Print message and terminate */

void err_quit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "(%s) Error - ", prog_name);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
    va_end(ap);
    exit(1);
}

/* Simple informational message 
 * Print message and return */

void info_msg(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stdout, "(%s) Info  - ", prog_name);
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, ".\n");
    va_end(ap);
    return;
}
