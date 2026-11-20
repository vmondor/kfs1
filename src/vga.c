#include "vga.h"
#include "io.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_CELLS   (VGA_WIDTH * VGA_HEIGHT)
#define VGA_MEMORY  ((volatile u16 *)0xB8000)

struct screen {
    u16     buf[VGA_CELLS];
    size_t  row;
    size_t  col;
    u8      color;
};

static struct screen  screens[VGA_SCREENS];
static struct screen *cur;
static int            cur_idx;

static inline u16 entry(char c, u8 attr)
{
    return (u16)(u8)c | ((u16)attr << 8);
}

static void update_cursor(void)
{
    u16 pos = cur->row * VGA_WIDTH + cur->col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

static void flush(void)
{
    for (size_t i = 0; i < VGA_CELLS; i++)
        VGA_MEMORY[i] = cur->buf[i];
}

static void put_cell(size_t x, size_t y, char c)
{
    u16 val = entry(c, cur->color);

    cur->buf[y * VGA_WIDTH + x] = val;
    VGA_MEMORY[y * VGA_WIDTH + x] = val;
}

static void scroll(void)
{
    for (size_t y = 1; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            cur->buf[(y - 1) * VGA_WIDTH + x] = cur->buf[y * VGA_WIDTH + x];

    for (size_t x = 0; x < VGA_WIDTH; x++)
        cur->buf[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = entry(' ', cur->color);

    cur->row = VGA_HEIGHT - 1;
    flush();
}

void vga_set_color(enum vga_color fg, enum vga_color bg)
{
    cur->color = fg | (bg << 4);
}


void vga_clear(void)
{
    for (size_t i = 0; i < VGA_CELLS; i++)
        cur->buf[i] = entry(' ', cur->color);
    cur->row = 0;
    cur->col = 0;
    flush();
    update_cursor();
}

void vga_init(void)
{
    for (int i = 0; i < VGA_SCREENS; i++) {
        screens[i].color = VGA_LIGHT_GREY | (VGA_BLACK << 4);
        screens[i].row = 0;
        screens[i].col = 0;
        for (size_t j = 0; j < VGA_CELLS; j++)
            screens[i].buf[j] = entry(' ', screens[i].color);
    }
    cur_idx = 0;
    cur = &screens[0];
    flush();
    update_cursor();
}

void vga_switch(int n)
{
    if (n < 0 || n >= VGA_SCREENS)
        return;
    cur_idx = n;
    cur = &screens[n];
    flush();
    update_cursor();
}

int vga_current(void)
{
    return cur_idx;
}

void vga_putchar(char c)
{
    if (c == '\n') {
        cur->col = 0;
        cur->row++;
    } else if (c == '\r') {
        cur->col = 0;
    } else if (c == '\t') {
        cur->col = (cur->col + 8) & ~7u;
    } else if (c == '\b') {
        if (cur->col > 0) {
            cur->col--;
            put_cell(cur->col, cur->row, ' ');
        }
    } else {
        put_cell(cur->col, cur->row, c);
        cur->col++;
    }

    if (cur->col >= VGA_WIDTH) {
        cur->col = 0;
        cur->row++;
    }
    if (cur->row >= VGA_HEIGHT)
        scroll();
    update_cursor();
}

void vga_puts(const char *s)
{
    while (*s)
        vga_putchar(*s++);
}
