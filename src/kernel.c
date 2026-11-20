#include "vga.h"
#include "printk.h"
#include "keyboard.h"

static void banner(int n)
{
    static const enum vga_color colors[VGA_SCREENS] = {
        VGA_LIGHT_GREEN, VGA_LIGHT_CYAN, VGA_YELLOW, VGA_LIGHT_MAGENTA
    };

    vga_switch(n);
    vga_set_color(colors[n], VGA_BLACK);
    if (n == 0)
        printk("42\n");
    printk("=== Screen %d ===\n", n + 1);
    vga_set_color(VGA_WHITE, VGA_BLACK);
    printk("F1-F4: switch screen | ESC: clear this screen\n> ");
}

void kmain(void)
{
    vga_init();

    for (int i = VGA_SCREENS - 1; i >= 0; i--)
        banner(i);

    for (;;) {
        int k = kbd_poll();

        if (k < 0) {
            __asm__ volatile ("pause");
            continue;
        }
        if (k >= KEY_F1 && k < KEY_F1 + VGA_SCREENS)
            vga_switch(k - KEY_F1);
        else if (k == 27)
            vga_clear();
        else if (k < 0x100)
            vga_putchar((char)k);
    }
}
