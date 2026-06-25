global context_switch
extern process_table
extern current_process

section .text

context_switch:
    pusha ;push eax, ecx, edx, ebx, esp, ebp, esi, edi

    mov eax, [current_process] ;process_table[current_process].esp = esp
    imul eax, 4144 ;4 bytes of ID + 28 bytes of registers + 4 of ESP + 4 of EIP + 4 + CR3 + 4 of state + 4096 of stack = 4144 bytes
    add eax, process_table
    add eax, 32 ;offset to esp
    mov [eax], esp ;save current esp

    ;next
    mov eax, [current_process]
    imul eax, 4144
    add eax, process_table
    add eax, 32
    mov esp, [eax] ;load new value

    popa
    ret

section .note.GNU-stack noalloc noexec nowrite progbits