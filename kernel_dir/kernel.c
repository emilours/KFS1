#include "../includes/kfs1.h"

void init_screens(void) {
    /* ÉCRAN 0 - Principal / Informations */
    screen_switch(0);
    screen_set_color(0x0F); /* Blanc brillant */
    ft_printf("=== KFS-1 Kernel - Main Screen ===\n\n");
    printk_info("Kernel initialized successfully.\n");
    printk_warn("This is a warning message.\n");
    printk_err("This is an error message.\n");
    ft_printf("\nTest: %d\n", 42);
    ft_printf("String test: %s\n", "Hello from screen 1");
    
    /* ÉCRAN 1 - Logs système */
    screen_switch(1);
    screen_set_color(0x0A); /* Vert clair */
    ft_printf("=== System Logs ===\n\n");
    ft_printf("[INFO] Keyboard driver initialized\n");
    ft_printf("[INFO] Screen manager initialized\n");
    ft_printf("[INFO] 4 virtual screens available\n");
    
    /* ÉCRAN 2 - Moniteur / Stats */
    screen_switch(2);
    screen_set_color(0x0E); /* Jaune */
    ft_printf("=== System Monitor ===\n\n");
    ft_printf("CPU: Unknown\n");
    ft_printf("Memory: VGA Text Mode\n");
    ft_printf("Resolution: 80x25\n");
    ft_printf("Screens: %d\n", NUM_SCREENS);
    
    /* ÉCRAN 3 - Debug / Console libre */
    screen_switch(3);
    screen_set_color(0x0C); /* Rouge clair */
    ft_printf("=== Debug Console ===\n\n");
    ft_printf("Free console for testing\n");
    ft_printf("Type here...\n\n");
    
    /* Retour à l'écran 0 par défaut */
    screen_switch(0);
}

void main(void) {
    /* Initialisation */
    screen_clear();
    keyboard_init();
    
    /* Préparer tous les écrans */
    init_screens();
    
    /* Boucle principale */
    while (1) {
        keyboard_poll_once();
    }
}