#ifndef STRING_H
#define STRING_H
#include <stddef.h>

int     ft_atoi(const char *str);
void    *ft_memset(void *s, int c, size_t n);
void    *ft_memcpy(void *dest, const void *src, size_t n);
size_t  ft_strlen(const char *str);
int     ft_strcmp(const char *s1, const char *s2);
char    *strcpy(char *dest, const char *src);

#endif
