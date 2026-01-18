#include "../../includes/kfs1.h"

/* VGA text mode constants */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
// #define SCROLL_BUFFER_LINES 100
//#define VGA_MEMORY 0xB8000

// static uint16_t scroll_buffer[SCROLL_BUFFER_LINES][VGA_WIDTH];
// static int scroll_buffer_start = 0;
// static int scroll_buffer_count = 0;

// Pointers to video memory
static unsigned short *video_memory = (unsigned short *)VGA_MEMORY;

static void vga_update_cursor(void);


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
    vga_update_cursor();
}

static inline void outb(unsigned short port, unsigned char value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Will calculate the current cursor position and update the hardware cursor
static void vga_update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Scroll the screen up by one line
static void vga_scroll(void) {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[(y - 1) * VGA_WIDTH + x] =
                VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }

    // Clear last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            (uint16_t)current_color << 8 | ' ';
    }

    cursor_y = VGA_HEIGHT - 1;
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

// show a character at the current cursor position
void screen_putc(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        screen_putchar(' ', cursor_x, cursor_y, current_color);
    } else {
        screen_putchar(c, cursor_x, cursor_y, current_color);
        cursor_x++;
    }
    
    // Line wrap
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // Scroll when reaching bottom
    if (cursor_y >= VGA_HEIGHT) {
        vga_scroll();
    }

    vga_update_cursor();
}

// Show a string at the current cursor position
void screen_puts(const char *s) {
    int i = 0;
    while (s[i] != '\0') {
        screen_putc(s[i]);
        i++;
    }
}

void screen_set_color(unsigned char color) {
    current_color = color;
}


void keyboard_handle_arrow_up(void) {
    if (cursor_y > 0) {
        cursor_y--;
        vga_update_cursor();
    }
}

void keyboard_handle_arrow_down(void) {
    if (cursor_y < VGA_HEIGHT - 1) {
        cursor_y++;
        vga_update_cursor();
    } else {
        vga_scroll();
    }
}

void keyboard_handle_arrow_left(void) {
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    }
    vga_update_cursor();
}

void keyboard_handle_arrow_right(void) {
    if (cursor_x < VGA_WIDTH - 1) {
        cursor_x++;
    } else if (cursor_y < VGA_HEIGHT - 1) {
        cursor_x = 0;
        cursor_y++;
    }
    vga_update_cursor();
}


void keyboard_handle_page_up(void) {
    // Scroll up one page
    if (cursor_y > 0) cursor_y = 0;
    vga_update_cursor();
}

void keyboard_handle_page_down(void) {
    // Scroll down one page
    cursor_y = VGA_HEIGHT - 1;
    vga_scroll();
    vga_update_cursor();
}