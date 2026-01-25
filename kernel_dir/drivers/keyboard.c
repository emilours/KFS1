#include "../../includes/kfs1.h"

static int extended_scancode = 0;

// Arrow key scancodes
#define SCANCODE_UP    0x48
#define SCANCODE_DOWN  0x50
#define SCANCODE_LEFT  0x4B
#define SCANCODE_RIGHT 0x4D

#define SCANCODE_F1  0x3B
#define SCANCODE_F2  0x3C
#define SCANCODE_F3  0x3D
#define SCANCODE_F4  0x3E

// Page Up/Down for scrolling
#define SCANCODE_PGUP  0x49
#define SCANCODE_PGDN  0x51

/* Very small keyboard driver in polling mode using scancode set 1.
 * - provides keyboard_init(), keyboard_shutdown(), keyboard_poll_once()
 * - calls keyboard_press_key(char) when a printable key is pressed
 */

/* Low-level port access */
// Read a byte from the specified port
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* Scancode set 1 -> ASCII (no international layout). 0 means unmapped/non-printable. */
static const char scancode_map[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', /* 0x00 - 0x0f */
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,  /* 0x10 - 0x1f (0=ctrl) */
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\','z','x', /* 0x20 - 0x2f */
    'c','v','b','n','m',',','.','/', 0,  '*', 0,  ' ',   /* 0x30 - 0x3b */
    /* rest zeros */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x3c - 0x5b */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                 /* fill to 128 */
};

/* Uppercase map for shifted characters (only common ones) */
static const char scancode_map_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~', 0,'|','Z','X',
    'C','V','B','N','M','<','>','?', 0,'*',0,' ', /* rest 0 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int shift_down = 0;
static int ctrl_down = 0;
static int alt_down = 0;

/* keyboard init/shutdown (polling version only) */
void keyboard_init() {
    printk_info("Keyboard driver initialized.\n");
    shift_down = 0;
    ctrl_down = 0;
    alt_down = 0;
}

void keyboard_shutdown() {
    printk_info("Keyboard driver shutdown.\n");
}

/* Called by this driver when a printable key is recognized.
 * You may override/implement this function elsewhere.
 */
void keyboard_press_key(char key) {
    /* default behaviour: print the key */
    ft_printf("%c", key);
}

/* Check if keyboard output buffer has data */
static int keyboard_scancode_available() {
    unsigned char status = inb(0x64);
    return status & 0x01;
}

/* Read one scancode from 0x60 */
static unsigned char keyboard_read_scancode() {
    return inb(0x60);
}


int keyboard_poll_once(void) {
    if (!keyboard_scancode_available())
        return 0;

    unsigned char sc = keyboard_read_scancode();

    if (sc == 0xE0) {
        extended_scancode = 1;
        return 1;
    }

    int released = sc & 0x80;
    unsigned char code = sc & 0x7F;

    /* Handle modifier keys (Shift, Ctrl, Alt) */
    
    if (code == 0x2A || code == 0x36) {
        if (released) shift_down = 0;
        else shift_down = 1;
        extended_scancode = 0;
        return 1;
    }

    if (code == 0x1D) { 
        if (released) ctrl_down = 0; 
        else ctrl_down = 1; 
        extended_scancode = 0; 
        return 1; 
    }
    
    if (code == 0x38) { 
        if (released) alt_down = 0; 
        else alt_down = 1; 
        extended_scancode = 0; 
        return 1; 
    }

    /* Ignore key release for other keys */
    if (released) {
        extended_scancode = 0;
        return 1;
    }

    /* Handle F1-F4 for screen switching */
    if (code >= SCANCODE_F1 && code <= SCANCODE_F4) {
        int idx = code - SCANCODE_F1;
        if (idx < NUM_SCREENS) {
            screen_switch(idx);
        }
        extended_scancode = 0;
        return 1;
    }

    /* Handle extended scancodes (arrows, etc.) */
    if (extended_scancode) {
        extended_scancode = 0;

        /* handle extended Ctrl / Alt (right Ctrl / right Alt) */
        if (code == 0x1D) { ctrl_down = 1; return 1; }
        if (code == 0x38) { alt_down = 1; return 1; }

        switch (code) {
            case SCANCODE_UP:
                keyboard_handle_arrow_up();
                return 1;
            case SCANCODE_DOWN:
                keyboard_handle_arrow_down();
                return 1;
            case SCANCODE_LEFT:
                keyboard_handle_arrow_left();
                return 1;
            case SCANCODE_RIGHT:
                keyboard_handle_arrow_right();
                return 1;
            case SCANCODE_PGUP:
                keyboard_handle_page_up();
                return 1;
            case SCANCODE_PGDN:
                keyboard_handle_page_down();
                return 1;
        }
        return 1;
    }

    if ((ctrl_down || alt_down) && (code >= 0x02 && code <= 0x05)) {
        int idx = code - 0x02; /* 0x02->0, 0x03->1, 0x04->2, 0x05->3 */
        if (idx >= 0 && idx < NUM_SCREENS) {
            screen_switch(idx);
            return 1;
        }
    }

    /* Handle printable characters */
    char c = 0;
    if (shift_down) 
        c = scancode_map_shift[code];
    else 
        c = scancode_map[code];

    if (c) {
        keyboard_press_key(c);
    } else {
        /* Non-printable, handle Enter / Backspace */
        if (code == 0x1C) { /* Enter */
            keyboard_press_key('\n');
        } else if (code == 0x0E) { /* Backspace */
            keyboard_press_key('\b');
        }
    }
    
    return 1;
}