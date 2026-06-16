#include "vga.h"

void kernel_main() {
    clear_vga();
    print_vga("Keti OS - By Aadithya-19");
    while(1) {}
}