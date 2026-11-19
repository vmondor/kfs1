#include "vga.h"
#include "printk.h"

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

    vga_set_color(VGA_WHITE, VGA_BLACK);
    printk("char   : %c\n", 'A');
    printk("string : %s | [%10s] | %s\n", "kfs1", "right", (char *)0);
    printk("int    : %d | %d | %d\n", 42, -42, -2147483647 - 1);
    printk("uint   : %u | %u\n", 0, 4294967295u);
    printk("hex    : %x | %X | %08x\n", 255, 255, 0xBEEF);
    printk("ptr    : %p\n", (void *)kmain);
    printk("percent: 100%%\n");
    printk("width  : [%5d] [%05d] [%-3d]\n", 42, 42, 7);
}
