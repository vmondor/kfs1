#include "keyboard.h"
#include "io.h"

#define KBD_DATA    0x60
#define KBD_STATUS  0x64

static const char map_normal[0x3A] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',0,
    '*',0,' '
};

static const char map_shift[0x3A] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?',0,
    '*',0,' '
};

static int shift;
static int caps;
static int extended;

static int is_letter(char c)
{
    return c >= 'a' && c <= 'z';
}

int kbd_poll(void)
{
    if ((inb(KBD_STATUS) & 0x21) != 0x01)
        return -1;

    u8 sc = inb(KBD_DATA);

    if (sc == 0xE0) {
        extended = 1;
        return -1;
    }
    if (extended) {
        extended = 0;
        return -1;
    }

    if (sc & 0x80) {
        sc &= 0x7F;
        if (sc == 0x2A || sc == 0x36)
            shift = 0;
        return -1;
    }

    if (sc == 0x2A || sc == 0x36) {
        shift = 1;
        return -1;
    }
    if (sc == 0x3A) {
        caps = !caps;
        return -1;
    }
    if (sc >= 0x3B && sc <= 0x3E)
        return KEY_F1 + (sc - 0x3B);

    if (sc >= 0x3A)
        return -1;

    char c = map_normal[sc];
    if (c == 0)
        return -1;

    int upper = is_letter(c) ? (shift ^ caps) : shift;
    return upper ? map_shift[sc] : c;
}
