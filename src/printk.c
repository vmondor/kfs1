#include <stdarg.h>
#include "printk.h"
#include "vga.h"
#include "types.h"

static void print_num(u32 n, u32 base, int upper, int neg, int width, char pad)
{
    static const char lower_digits[] = "0123456789abcdef";
    static const char upper_digits[] = "0123456789ABCDEF";
    const char *digits = upper ? upper_digits : lower_digits;
    char buf[34];
    int i = 0;
    int len;

    if (n == 0)
        buf[i++] = '0';
    while (n) {
        buf[i++] = digits[n % base];
        n /= base;
    }

    len = i + neg;
    if (neg && pad == '0')
        vga_putchar('-');
    while (len < width) {
        vga_putchar(pad);
        len++;
    }
    if (neg && pad != '0')
        vga_putchar('-');
    while (i--)
        vga_putchar(buf[i]);
}

static void print_str(const char *s, int width)
{
    int len = 0;

    if (!s)
        s = "(null)";
    while (s[len])
        len++;
    while (len < width) {
        vga_putchar(' ');
        len++;
    }
    vga_puts(s);
}

void printk(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            vga_putchar(*fmt);
            continue;
        }
        fmt++;

        char pad = ' ';
        int width = 0;

        if (*fmt == '0') {
            pad = '0';
            fmt++;
        }
        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }

        switch (*fmt) {
        case 'c':
            vga_putchar((char)va_arg(ap, int));
            break;
        case 's':
            print_str(va_arg(ap, const char *), width);
            break;
        case 'd':
        case 'i': {
            s32 v = va_arg(ap, s32);
            if (v < 0)
                print_num(-(u32)v, 10, 0, 1, width, pad);
            else
                print_num((u32)v, 10, 0, 0, width, pad);
            break;
        }
        case 'u':
            print_num(va_arg(ap, u32), 10, 0, 0, width, pad);
            break;
        case 'x':
            print_num(va_arg(ap, u32), 16, 0, 0, width, pad);
            break;
        case 'X':
            print_num(va_arg(ap, u32), 16, 1, 0, width, pad);
            break;
        case 'p':
            vga_puts("0x");
            print_num((u32)va_arg(ap, void *), 16, 0, 0, 8, '0');
            break;
        case '%':
            vga_putchar('%');
            break;
        case '\0':
            va_end(ap);
            return;
        default:
            vga_putchar('%');
            vga_putchar(*fmt);
            break;
        }
    }
    va_end(ap);
}
