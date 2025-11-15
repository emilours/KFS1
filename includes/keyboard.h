#ifndef KEYBOARD_H
#define KEYBOARD_H
// Keyboard I/O ports ref: https://wiki.osdev.org/I8042_PS/2_Controller
// The PS/2 Controller itself uses 2 IO ports (IO ports 0x60 and 0x64)
#define KEYBOARD_DATA_PORT 0x60 // keyboard data port
#define KEYBOARD_STATUS_PORT 0x64 // keyboard status port

// Keyboard scancode table
static unsigned char SCANCODE[128] =
{
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  // Ctrl
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  // LShift
    '\\','z','x','c','v','b','n','m',',','.','/',
    0, // RShift
    '*',
    0, // Alt
    ' ', // Space
    // function keys (F1–F10):
    0,0,0,0,0,0,0,0,0,0,
};

char    getScancode(void);
char    getchar(void);

#endif