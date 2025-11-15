#ifndef SCREEN_H
#define SCREEN_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#define COLOR_BLACK 0
#define COLOR_WHITE 15
#define COLOR_LIGHT_GREY 7
#define COLOR_GREEN 2
#define COLOR_RED 4
#define COLOR_YELLOW 14

void screen_clear();
void screen_putchar(char c, int x, int y, unsigned char color);
void screen_print(const char *str, int x, int y, unsigned char color);
void screen_putc(char c);
void screen_puts(const char *s);

#endif