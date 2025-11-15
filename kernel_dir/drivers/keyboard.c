#include "../../includes/kfs1.h"

// This function is used for reading user entry's scancode
char    getScancode(void) {
    char c=0;
    do {
        if(inb(0x60)!=c) {
            c = inb(0x60);
            if (c > 0)
                return c;
        }
    } while (1);
}
// This function is used for reading user entry
char    getchar(void) {
    return SCANCODE[getScancode() + 1];
}