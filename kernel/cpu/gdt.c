#include "gdt.h"

struct gdt_entry gdt[6];
struct tss_entry tss;
struct gdt_descriptor gdt_desc;
unsigned char kernel_stack[4096];

void gdt_set_entry(int i, unsigned int base, unsigned int limit, unsigned char access, unsigned char flags){
        gdt[i].low_limit = limit & 0xFFFF; //lower 16
        gdt[i].low_base = base & 0xFFFF; //lower 16
        gdt[i].mid_base = (base >> 16) & 0xFF; //mid 8
        gdt[i].access_byte = access; 
        gdt[i].lim_flag_high = ((limit >> 16) & 0x0F) | (flags << 4); //upper 4 only and upper 4 of flag
        gdt[i].high_base = (base >> 24) & 0xFF; //upper 8
}
void tss_init(){
    tss.prev_tss = 0;
    tss.esp0 = (unsigned int)kernel_stack + 4096; 
    tss.ss0 = 0x10; 
    tss.esp1 = 0;
    tss.ss1 = 0;
    tss.esp2 = 0;
    tss.ss2 = 0;
    tss.cr3 = 0;
    tss.eip = 0;
    tss.eflags = 0;
    tss.eax = 0; 
    tss.ecx = 0; 
    tss.edx = 0; 
    tss.ebx = 0;
    tss.esp = 0;
    tss.ebp = 0; 
    tss.esi = 0; 
    tss.edi = 0;
    tss.es = 0;
    tss.cs = 0; 
    tss.ss = 0; 
    tss.ds = 0; 
    tss.fs = 0; 
    tss.gs = 0;
    tss.ldt = 0;
    tss.trap = 0;
    tss.iomap_base = 0;

    unsigned int tss_base = (unsigned int)&tss;
    unsigned int tss_limit = sizeof(struct tss_entry);
    gdt_set_entry(5, tss_base, tss_limit, 0x89, 0x0);
}

void gdt_init(){
    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0x0, 0xFFFFF, 0x9A, 0xC); //cs
    gdt_set_entry(2, 0x0, 0xFFFFF, 0x92, 0xC); //ds
    gdt_set_entry(3, 0x0, 0xFFFFF, 0xFA, 0xC); // user code
    gdt_set_entry(4, 0x0, 0xFFFFF, 0xF2, 0xC);
    tss_init();
    gdt_desc.size = (sizeof(struct gdt_entry) * 6) - 1;
    gdt_desc.address = (unsigned int)&gdt;
    gdt_load();
}

