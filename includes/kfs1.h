#ifndef KFS1_H
#define KFS1_H

# include "string.h"
# include "screen.h"
# include "stdio.h"
# include "stdlib.h"
# include "keyboard.h"

#define printk_err(msg)   screen_set_color(COLOR_RED);    ft_printf(msg); screen_set_color(COLOR_WHITE);
#define printk_warn(msg)  screen_set_color(COLOR_YELLOW); ft_printf(msg); screen_set_color(COLOR_WHITE);
#define printk_info(msg)  ft_printf(msg);
#define printk(msg)       ft_printf(msg);

#include <stddef.h>
#endif