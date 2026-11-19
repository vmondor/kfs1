NAME     = kernel.bin
ISO      = kernel.iso

ASM      = nasm
CC       = gcc
LD       = ld

ASMFLAGS = -f elf32
CFLAGS   = -m32 -ffreestanding -fno-builtin -fno-stack-protector \
           -fno-pic -fno-pie -nostdlib -nodefaultlibs \
           -Wall -Wextra -Werror -Isrc
LDFLAGS  = -m elf_i386 -T linker.ld --no-warn-rwx-segments

SRC_ASM  = src/boot.asm
SRC_C    = $(wildcard src/*.c)
OBJ      = $(SRC_ASM:.asm=.o) $(SRC_C:.c=.o)
HEADERS  = $(wildcard src/*.h)

all: $(ISO)

$(NAME): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

src/%.o: src/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

src/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(ISO): $(NAME) grub.cfg
	mkdir -p iso/boot/grub
	cp $(NAME) iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -d /usr/lib/grub/i386-pc --compress=xz --fonts= --locales= --themes= -o $(ISO) iso 2>/dev/null

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(ISO)
	rm -f iso/boot/kernel.bin

re: fclean all

.PHONY: all run clean fclean re
