#include "../includes/kfs1.h"

void main() {
    screen_clear();
    // screen_print("42", 0, 0, 0x07);
    // ft_printf("Hello, World!\n");
    // ft_printf("Decimal: %d\n", -12345);
    // ft_printf("Unsigned: %u\n", 12345U);
    // ft_printf("Hexadecimal: %x\n", 0xABCD);
    // ft_printf("String: %s\n", "Test string");
    // ft_printf("Character: %c\n", 'A');
    ft_printf("%d", 42);
    printk_info("Kernel initialized successfully.\n");
    printk_warn("This is a warning message.\n");
    printk_err("This is an error message.\n");

    printk_info("keyboard test start\n");
    keyboard_init();
    while (1) {
        keyboard_poll_once();
    }
}