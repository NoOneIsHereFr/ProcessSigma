#ifndef VERBOSE_H
#define VERBOSE_H

#include <stdio.h>

#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"

int verbosemsg(char *msg, char *type);
extern void verbose_wide(const wchar_t *wide_msg, const char *type);

#endif