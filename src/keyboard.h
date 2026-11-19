#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEY_F1  0x100
#define KEY_F2  0x101
#define KEY_F3  0x102
#define KEY_F4  0x103

/* Retourne -1 s'il n'y a rien à lire, sinon un caractère ASCII
   ou une touche spéciale (>= 0x100). */
int kbd_poll(void);

#endif
