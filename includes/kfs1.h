#ifndef KFS1_H
#define KFS1_H

#include <stddef.h>
#include <stdint.h>

// Global variables for cursor position
static int cursor_x = 0;
static int cursor_y = 15;
static unsigned char current_color = 0x07; // White on black by default

// Cursor functions
static inline unsigned char inb(unsigned short port) {
    unsigned char value;
    asm volatile("inb %w1, %0" : "=a" (value) : "Nd" (port));
    return value;
}

static inline void outb(unsigned short port, unsigned char value) {
    asm volatile("outb %b0, %w1" : : "a" (value), "Nd" (port));
}

# include "string.h"
# include "screen.h"
# include "keyboard.h"

#endif 