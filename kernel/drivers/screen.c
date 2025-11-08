#include "../includes/kfs1.h"

/* VGA text mode constants */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Pointers to video memory
static unsigned short *video_memory = (unsigned short *)VGA_MEMORY;

// Global variables for cursor position
static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = 0x07; // White on black by default

// Clear the screen
void screen_clear() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            int position = y * VGA_WIDTH + x;
            video_memory[position] = ' ' | (current_color << 8);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Show a character at a given position with a given color
void screen_putchar(char c, int x, int y, unsigned char color) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT)
        return;
    
    int position = y * VGA_WIDTH + x;
    video_memory[position] = c | (color << 8);
}

// Show a string at a given position with a given color
void screen_print(const char *str, int x, int y, unsigned char color) {
    int i = 0;
    while (str[i] != '\0') {
        screen_putchar(str[i], x + i, y, color);
        i++;
    }
}