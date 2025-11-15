#include "../includes/kfs1.h"

void main() {
    screen_clear();
    enable_cursor(cursor_x, cursor_y);
    // screen_print("This is a test kajsdhakjhdakjshdaj", 0, 0, current_color);
    char c = getchar();
    screen_putchar(c,0,0,current_color);
    // disable_cursor();
}