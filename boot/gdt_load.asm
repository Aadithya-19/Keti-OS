global gdt_load
extern gdt_desc ; the descriptor struct from gdt.c

section .text

gdt_load:
    lgdt [gdt_desc] ; load the GDT descriptor into the CPU

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    jmp 0x08:flush
    flush:
        ret
section .note.GNU-stack noalloc noexec nowrite progbits
