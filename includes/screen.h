#ifndef SCREEN_H
#define SCREEN_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// CouleursColors
#define COLOR_BLACK 0
#define COLOR_WHITE 15
#define COLOR_LIGHT_GREY 7
#define COLOR_GREEN 2
#define COLOR_RED 4
#define COLOR_YELLOW 14

/* VGA text mode constants */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Cursor control ports
#define VGA_CURSOR_PORT_COMMAND 0x3D4
#define VGA_CURSOR_PORT_DATA 0x3D5

// Cursor functions
void    enable_cursor(unsigned char cursor_start, unsigned char cursor_end);
void    update_cursor(int x, int y);
uint16_t    get_cursor_position(void);
void    disable_cursor();

// Pointers to video memory
static unsigned short *video_memory = (unsigned short *)VGA_MEMORY;

void screen_clear();
void screen_putchar(char c, int x, int y, unsigned char color);
void screen_print(const char *str, int x, int y, unsigned char color);

#endif