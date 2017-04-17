#ifndef UNP_INCLUDE_ERROR_H
#define UNP_INCLUDE_ERROR_H

#include	<stdarg.h>		/* ANSI C header file */
#include	<syslog.h>		/* for syslog() */

void error(const char *fmt, ...);
void error_sys(const char *fmt, ...);
void fatal(const char *fmt, ...);
void fatal_sys(const char *fmt, ...);

#endif
