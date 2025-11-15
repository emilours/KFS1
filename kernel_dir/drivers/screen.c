#include "../../includes/kfs1.h"

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
    uint16_t pos = i;
    update_cursor(pos % VGA_WIDTH, pos / VGA_WIDTH);
}

// static inline uint8_t inb(uint16_t port)
// {
//     uint8_t ret;
//     __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
//     return ret;
// }

// static inline void outb(uint16_t port, uint8_t val)
// {
//     __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
// }

void enable_cursor(unsigned char cursor_start, unsigned char cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// To get coordinates: x = pos % VGA_WIDTH y = pos / VGA_WIDTH
void update_cursor(int x, int y)
{
	uint16_t pos = y * 80 + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

// Disable the cursor
void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}
