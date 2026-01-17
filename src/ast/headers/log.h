#ifndef LOG_H
#define LOG_H

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

extern void yyerror(const char* msg);

void init_log(const char *filename);
void close_log(void);
void log_message(const char *format, ...);
void halt(const char *format, ...);

#endif

