global enter_usermode

section .text

enter_usermode:
    mov edx, [esp+4] 

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23
    push 0x800000
    pushf
    pop eax
    or eax, 0x200
    push eax
    push 0x1B
    push edx

    iret

section .note.GNU-stack noalloc noexec nowrite progbits
