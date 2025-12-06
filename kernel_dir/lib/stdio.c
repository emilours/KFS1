#include "../../includes/kfs1.h"

// show a character on screen
int ft_putchar(int c) {
    screen_putc((char)c);
    return c;
}

// show a string on screen
int ft_puts(const char *s) {
    screen_puts(s);
    return 1;
}

int ft_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    int i = 0;
    int count = 0;
    char buffer[32];  // Buffer for numeric conversions
    
    while (format[i] != '\0') {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            
            switch (format[i]) {
                case 'd': // Decimal signed
                case 'i':
                {
                    int value = va_arg(args, int);
                    ft_itoa(value, buffer, 10);
                    ft_puts(buffer);
                    count += ft_strlen(buffer);
                    break;
                }
                
                case 'u':  // Decimal unsigned
                {
                    unsigned int value = va_arg(args, unsigned int);
                    ft_uitoa(value, buffer, 10);
                    ft_puts(buffer);
                    count += ft_strlen(buffer);
                    break;
                }
                
                case 'x':  // Hexadecimal
                case 'X':
                {
                    unsigned int value = va_arg(args, unsigned int);
                    ft_uitoa(value, buffer, 16);
                    ft_puts(buffer);
                    count += ft_strlen(buffer);
                    break;
                }
                
                case 's':  // String
                {
                    char *str = va_arg(args, char*);
                    if (str == NULL)
                        str = "(null)";
                    ft_puts(str);
                    count += ft_strlen(str);
                    break;
                }
                
                case 'c':  // Character
                {
                    char c = (char)va_arg(args, int);
                    ft_putchar(c);
                    count++;
                    break;
                }
                
                case 'p':  // Pointer
                {
                    void *ptr = va_arg(args, void*);
                    ft_puts("0x");
                    ft_uitoa((unsigned int)ptr, buffer, 16);
                    ft_puts(buffer);
                    count += ft_strlen(buffer) + 2;
                    break;
                }
                
                case '%':  // Literal %
                {
                    ft_putchar('%');
                    count++;
                    break;
                }
                
                default:  // Unknown format
                {
                    ft_putchar('%');
                    ft_putchar(format[i]);
                    count += 2;
                    break;
                }
            }
            
        } else {
            // Normal character
            ft_putchar(format[i]);
            count++;
        }
        i++;
    }
    
    va_end(args);
    return count;
}