#ifndef SCREEN_H
#define SCREEN_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
//#define VGA_MEMORY 0xB8000
#define VGA_MEMORY ((volatile uint16_t *)0xB8000)

#define COLOR_BLACK 0
#define COLOR_WHITE 15
#define COLOR_LIGHT_GREY 7
#define COLOR_GREEN 2
#define COLOR_RED 4
#define COLOR_YELLOW 14

/* number of virtual screens (Ctrl/Alt + 1..4) */
#define NUM_SCREENS 4

void screen_clear();
void screen_putchar(char c, int x, int y, unsigned char color);
void screen_print(const char *str, int x, int y, unsigned char color);
void screen_putc(char c);
void screen_puts(const char *s);
void screen_set_color(unsigned char color);

/* switch to virtual screen id (0..NUM_SCREENS-1) */
void screen_switch(int screen_number);

void keyboard_handle_arrow_up(void);
void keyboard_handle_arrow_down(void);
void keyboard_handle_arrow_left(void);
void keyboard_handle_arrow_right(void);
void keyboard_handle_page_up(void);
void keyboard_handle_page_down(void);

/* À ajouter à la fin de votre screen.h */
void screen_display_indicator(void);
void screen_display_shortcuts(void);

#endif