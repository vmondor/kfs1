#include "vga.h"

void kmain(void)
{
    vga_init();

    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    vga_puts("42\n");

    vga_set_color(VGA_YELLOW, VGA_BLUE);
    vga_puts("Hello from kfs1\n");

    vga_set_color(VGA_WHITE, VGA_BLACK);
    for (int i = 0; i < 10; i++)
        vga_puts("scroll test\n");

    vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
    vga_puts("fin\n");
}
