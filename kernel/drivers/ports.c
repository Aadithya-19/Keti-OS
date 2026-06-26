#include "ports.h"

//sends one output byte to hardware
void outb(unsigned short port, unsigned char value) {
    __asm__ volatile ("outb %1, %0" : : "dN"(port), "a"(value));
}
//reads one input byte from hardware
unsigned char inb(unsigned short port) {
    unsigned char value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}