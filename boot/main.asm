[org 0x7C00]
bits 16

%define NEXT_LINE 0x0D, 0x0A

; basic starting point with Keti OS 
; so I will print my initial statement in the intro and 
; then I will use the looping condition to display it

start: jmp main

puts: push si
    push ax
    push bx
    push cx

.loop: lodsb
    or al, al ;all this does is modify the flag reg, so once the PC is done printing the intro statement, it will reach the conditional end
    jz .done ;to pop the stack elements

    mov ah, 0x0E; call bios interrupt
    mov ch, 0 ; set page number to 0
    int 0x10 ; calls the BIOS interrupt corresponding to videos

    jmp .loop

.done: pop cx
    pop ax
    pop bx
    pop si    
    ret

main: mov ax, 0 ; can't set ds/es directly
    mov ds, ax ;Allocated my AX to the Data Segment reg
    mov es, bx ;Allocated my BX to the Extra D Segment reg
    
    ; setup stack
    mov ss, cx ;Allocated my CX to the Stack Segment reg

    ; No reason not to use ax or even BX here, its just I started with bx here

    mov sp, 0x7C00 ;bios starting point (good theory to know)

    ; print intro message
    mov si, intro
    call puts
    mov si, me
    call puts

intro: db 'Meet Keti OS', NEXT_LINE, 0
me: db 'By Aadithya Vasudevan', 0

.halt:
    hlt
    jmp .halt

times 510-($-$$) db 0 
dw 0xAA55 ;bios ending 