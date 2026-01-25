#include "../../includes/kfs1.h"

/* VGA text mode constants */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
//#define VGA_MEMORY 0xB8000

/* Use the macro from includes */
static volatile uint16_t *video_memory = VGA_MEMORY;

static void vga_update_cursor(void);

/* Per-screen buffers and state */
static uint16_t screens[NUM_SCREENS][VGA_WIDTH * VGA_HEIGHT];
static int active_screen = 0;
static int screen_cursor_x[NUM_SCREENS];
static int screen_cursor_y[NUM_SCREENS];
static unsigned char screen_color[NUM_SCREENS];

/* Global variables reflect the active screen state */
static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = 0x07; // default white on black

/* helper to write cell to active buffer and VRAM (if active) */
static inline void write_cell(int screen, int pos, uint16_t cell) {
    screens[screen][pos] = cell;
    if (screen == active_screen) {
        video_memory[pos] = cell;
    }
}

static inline void outb(unsigned short port, unsigned char value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* Update hardware cursor using current (active) cursor_x/cursor_y */
static void vga_update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

/* Copy buffer id -> actual VGA memory and set cursor/color */
void screen_switch(int id) {
    if (id < 0 || id >= NUM_SCREENS) return;

    /* save current state */
    screen_cursor_x[active_screen] = cursor_x;
    screen_cursor_y[active_screen] = cursor_y;
    screen_color[active_screen] = current_color;

    /* switch */
    active_screen = id;

    /* restore state for new active */
    cursor_x = screen_cursor_x[active_screen];
    cursor_y = screen_cursor_y[active_screen];
    current_color = screen_color[active_screen];

    /* copy whole buffer to VGA memory */
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = screens[active_screen][i];
    }

    vga_update_cursor();
    
    /* Afficher les indicateurs après le switch */
    screen_display_indicator();
    screen_display_shortcuts();
}

/* Clear all screens and VRAM */
void screen_clear() {
    uint16_t blank = ' ' | (0x07 << 8);
    for (int s = 0; s < NUM_SCREENS; s++) {
        for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
            screens[s][i] = blank;
        }
        screen_cursor_x[s] = 0;
        screen_cursor_y[s] = 0;
        screen_color[s] = 0x07;
    }

    /* set VRAM from active buffer (which we just initialized) */
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = screens[active_screen][i];
    }

    cursor_x = 0;
    cursor_y = 0;
    current_color = 0x07;
    vga_update_cursor();
}

/* Scroll all screens up by one line (keep each buffer consistent) */
static void vga_scroll(void) {
    for (int s = 0; s < NUM_SCREENS; s++) {
        for (int y = 1; y < VGA_HEIGHT; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                int dst = (y - 1) * VGA_WIDTH + x;
                int src = y * VGA_WIDTH + x;
                screens[s][dst] = screens[s][src];
            }
        }
        /* clear last line */
        for (int x = 0; x < VGA_WIDTH; x++) {
            int pos = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
            screens[s][pos] = (uint16_t)screen_color[s] << 8 | ' ';
        }
    }

    /* Refresh VGA memory from active buffer */
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = screens[active_screen][i];
    }

    cursor_y = VGA_HEIGHT - 1;
}

/* Show a character at a given position with a given color */
void screen_putchar(char c, int x, int y, unsigned char color) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT)
        return;

    int position = y * VGA_WIDTH + x;
    uint16_t cell = (uint16_t)c | ((uint16_t)color << 8);
    write_cell(active_screen, position, cell);
}

/* Show a string at a given position with a given color */
void screen_print(const char *str, int x, int y, unsigned char color) {
    int i = 0;
    while (str[i] != '\0') {
        screen_putchar(str[i], x + i, y, color);
        i++;
    }
}

/* show a character at the current cursor position */
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

    /* Line wrap */
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    /* Scroll when reaching bottom */
    if (cursor_y >= VGA_HEIGHT) {
        /* ensure the active screen's color is up-to-date */
        screen_color[active_screen] = current_color;
        vga_scroll();
    } else {
        /* update only the modified cell in VRAM already done in screen_putchar */
        for (int i = 0; i < 0; i++); /* no-op to keep compact */
    }

    vga_update_cursor();
}

/* Show a string at the current cursor position */
void screen_puts(const char *s) {
    int i = 0;
    while (s[i] != '\0') {
        screen_putc(s[i]);
        i++;
    }
}

void screen_set_color(unsigned char color) {
    current_color = color;
    screen_color[active_screen] = color;
}

/* Arrow/page handlers */
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
    if (cursor_y > 0) cursor_y = 0;
    vga_update_cursor();
}

void keyboard_handle_page_down(void) {
    cursor_y = VGA_HEIGHT - 1;
    vga_scroll();
    vga_update_cursor();
}

/* Affiche l'indicateur d'écran actif en haut à droite */
void screen_display_indicator(void) {
    /* Sauvegarder l'état actuel */
    int old_x = cursor_x;
    int old_y = cursor_y;
    unsigned char old_color = current_color;
    
    /* Afficher l'indicateur */
    char buffer[20];
    int i = 0;
    buffer[i++] = ' ';
    buffer[i++] = 'S';
    buffer[i++] = 'c';
    buffer[i++] = 'r';
    buffer[i++] = 'e';
    buffer[i++] = 'e';
    buffer[i++] = 'n';
    buffer[i++] = ' ';
    buffer[i++] = '1' + active_screen;
    buffer[i++] = '/';
    buffer[i++] = '0' + NUM_SCREENS;
    buffer[i++] = ' ';
    buffer[i] = '\0';
    
    screen_print(buffer, VGA_WIDTH - 12, 0, 0x70); /* Noir sur gris clair */
    
    /* Restaurer l'état */
    cursor_x = old_x;
    cursor_y = old_y;
    current_color = old_color;
}

/* Affiche la barre de raccourcis en bas de l'écran */
void screen_display_shortcuts(void) {
    int old_x = cursor_x;
    int old_y = cursor_y;
    unsigned char old_color = current_color;
    
    screen_print("Ctrl+1:Main  Ctrl+2:Logs  Ctrl+3:Monitor  Ctrl+4:Debug", 
                 2, VGA_HEIGHT - 1, 0x70);
    
    cursor_x = old_x;
    cursor_y = old_y;
    current_color = old_color;
}