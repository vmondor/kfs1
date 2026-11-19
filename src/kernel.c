#include "vga.h"
#include "printk.h"
#include "keyboard.h"

void kmain(void)
{
    vga_init();

    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    printk("42\n");
    vga_set_color(VGA_WHITE, VGA_BLACK);
    printk("Tape quelque chose (F1-F4 = test touches speciales)\n> ");

    for (;;) {
        int k = kbd_poll();

        if (k < 0) {
            __asm__ volatile ("pause");
            continue;
        }
        if (k >= KEY_F1)
            printk("[F%d]", k - KEY_F1 + 1);
        else if (k == 27)
            vga_init();               /* ESC efface l'écran */
        else
            vga_putchar((char)k);
    }
}
