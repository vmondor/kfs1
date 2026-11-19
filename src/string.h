/* string.h */
#ifndef STRING_H
#define STRING_H
#include "types.h"

size_t  strlen(const char *s);
int     strcmp(const char *a, const char *b);
void    *memset(void *dst, int c, size_t n);
void    *memcpy(void *dst, const void *src, size_t n);

#endif
