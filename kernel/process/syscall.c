#include "syscall.h"
#include "display/vga.h"
#include "process/pcb.h"
#include "process/scheduler.h"
extern int current_process; 


void sys_read(){
    return;
}

void sys_write(const char *str, unsigned int len){
    print_vga(str);
}

void sys_exit(){
    process_table[current_process].state = PROCESS_EMPTY;
    schedule();
}

void syscall_handler(unsigned int syscall_num, unsigned int arg1, unsigned int arg2) {  
    switch(syscall_num){
        case SYS_WRITE:
            sys_write((const char *)arg1, arg2);
            break;
        case SYS_READ:
            sys_read();
            break;
        case SYS_EXIT:
            sys_exit();
            break;
        default:
            print_vga("unknown syscall\n");
            break;
    }
}
