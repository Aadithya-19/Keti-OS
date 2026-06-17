#include "vga.h"
#include "gdt.h"

void kernel_main() {
    gdt_init();
    clear_vga();
    print_vga("Keti OS\nBy Aadithya Vasudevan\nVersion 0.1");
    while(1) {}
}