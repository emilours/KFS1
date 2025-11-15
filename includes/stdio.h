#ifndef STDIO_H
#define STDIO_H

typedef __builtin_va_list va_list;
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

int ft_printf(const char *format, ...);
int ft_putchar(int c);
int ft_puts(const char *s);

#endif