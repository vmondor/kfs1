bits 32

section .multiboot
align 4
    dd 0x1BADB002                  ; magic multiboot
    dd 0x00000000                  ; flags
    dd -(0x1BADB002 + 0x00000000)  ; checksum

section .bss
align 16
stack_bottom:
    resb 16384                     ; 16 Ko de pile
stack_top:

section .text
global _start
extern kmain

_start:
    mov esp, stack_top             ; GRUB ne donne pas de pile
    call kmain
.hang:
    cli
    hlt
    jmp .hang
