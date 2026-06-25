//doctor ahh
#include "paging.h"
#include "display/vga.h"

unsigned int page_directory[1024] __attribute__((aligned(4096)));
unsigned int page_tables[4][1024] __attribute__((aligned(4096)));//this allows 16MB
//aligned keeps it at the 4KB boundary (hence 4096)
extern void load_paging();

void paging_init(){
    for (int i = 0; i < 1024; i++){
        page_directory[i] = 0x2;
    }
    for (int j = 0; j < 4; j++){
        for (int k = 0; k < 1024; k++){
            unsigned int frame = j*1024 + k;
            page_tables[j][k] = (frame * 4096) | 0x7;
        }
        page_directory[j] = ((unsigned int) page_tables[j]) | 0x7;
    }
    load_paging();
}

void page_fault_handler(unsigned int error_code) {
    unsigned int faulting_address;
    __asm__ volatile ("mov %%cr2, %0" : "=r"(faulting_address));
    print_vga("\nPAGE FAULT at ");
    print_hex_vga(faulting_address);
    print_vga(" error: ");
    print_hex_vga(error_code);
    print_char_vga('\n');
    while(1) {}
}