#include "pcb.h"

extern unsigned int page_directory[];
struct process process_table[MAX_PROCESS];
int current_process = 0;
int process_count = 0;

void pcb_create(void (*entry)()){
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (process_table[i].state == PROCESS_EMPTY){
            process_table[i].id = i+1;
            process_table[i].eip = (unsigned int)entry;
            
            process_table[i].esp = (unsigned int)process_table[i].stack + 4096;
            if (entry != 0) {
                process_table[i].eip = (unsigned int)entry;
                
                // set up initial stack frame
                // ESP points to top of stack, push 8 zeros (for popa: edi,esi,ebp,esp,ebx,edx,ecx,eax)
                unsigned int *stack_top = (unsigned int *)(process_table[i].esp);
                
                stack_top--;  *stack_top = 0;          // EAX
                stack_top--;  *stack_top = 0;          // ECX
                stack_top--;  *stack_top = 0;          // EDX
                stack_top--;  *stack_top = 0;          // EBX
                stack_top--;  *stack_top = 0;          // ESP (ignored by popa)
                stack_top--;  *stack_top = 0;          // EBP
                stack_top--;  *stack_top = 0;          // ESI
                stack_top--;  *stack_top = 0;          // EDI
                stack_top--;  *stack_top = (unsigned int)entry;  // return address = entry point
                
                // update ESP to point to the new stack top
                process_table[i].esp = (unsigned int)stack_top;
                
                process_table[i].state = PROCESS_READY;
            }
            process_table[i].cr3 = (unsigned int)page_directory;
            for (int j = 0; j < 7; j++) {
                process_table[i].regs[j] = 0;
            }
            process_table[i].state = PROCESS_READY;
            process_count++;
            return;
        }
    }
}

void pcb_init() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        process_table[i].state = PROCESS_EMPTY;
        process_table[i].id = 0;
    }
    process_count = 0;
    current_process = 0;
}