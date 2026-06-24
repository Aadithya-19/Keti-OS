#include "keyboard.h"
#include "display/vga.h"
#include "cpu/idt.h"
#include "ports.h"
#include "shell/shell.h"

extern void keyboard_isr();
static int shift_pressed = 0;
static int arrow = 0;

static char scancode_table[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};
//reflects shift
static char scancode_shift[128] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' '
};


void keyboard_handler(){
    unsigned char scancode = inb(0x60);  
    
    if (scancode == 0xE0) {
        arrow = 1;
        outb(0x20, 0x20);
        return;
    }
    
    if (arrow) {
        arrow = 0;
        if (scancode == 0x48) {            // up arrow
            shell_up();
        }
        else if (scancode == 0x50) {       // down arrow
            shell_down();
        }
        outb(0x20, 0x20);
        return;
    }

    //shift press
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
    }
    // shift release (since it isnt stick keys)
    else if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
    }
    if (scancode == 0x0E) {
        shell_handle_input('\b');
    } 
    else if(scancode < 128){
        char c = shift_pressed ? scancode_shift[scancode] : scancode_table[scancode];
        if (c != 0){
            shell_handle_input(c);
        }
    }

    outb(0x20, 0x20);
}

void keyboard_init(){
    //register vector 33
    idt_fill(33, (unsigned int)keyboard_isr);
}