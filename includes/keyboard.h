#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init(void);
int  keyboard_poll_once(void);
void keyboard_shutdown(void);
void keyboard_press_key(char key);

#endif