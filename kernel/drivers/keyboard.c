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

//Handles all basic keyboard inputs 
void keyboard_handler(){
    unsigned char scancode = inb(0x60);
    //both arrows register 0xE0 first
    if (scancode == 0xE0) {
        arrow = 1;
        outb(0x20, 0x20);
        return;
    }
    //arrow handler
    if (arrow) {
        arrow = 0;
        if (scancode == 0x48) {
            shell_up(); //up arrow
        }
        else if (scancode == 0x50) {
            shell_down(); //down arrow
        }
        outb(0x20, 0x20);
        return;
    }

    // shift press/release
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
    }
    else if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
    }

    //F11/F12 scrollback - these are NOT extended, so they go here
    if (scancode == 0x57) { //F11 - scroll up
        scrollback_up();
    }
    else if (scancode == 0x58) {//F12 - scroll down
        scrollback_down();
    }
    else if (scancode == 0x0E) {
        shell_handle_input('\b');
    }
    //printable ASCII
    else if (scancode < 128) {
        char c = shift_pressed ? scancode_shift[scancode] : scancode_table[scancode];
        if (c != 0) {
            shell_handle_input(c);
        }
    }

    outb(0x20, 0x20);
}
void keyboard_init(){
    //register vector 33
    idt_fill(33, (unsigned int)keyboard_isr);
}