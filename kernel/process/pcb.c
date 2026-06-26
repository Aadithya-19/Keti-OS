#include "pcb.h"

extern unsigned int page_directory[];
struct process process_table[MAX_PROCESS];
int current_process = 0;
int process_count = 0;


//Creates the Process Controls and basically picks a process from the process table and checks if its empty.
//if it is, it just assigns an ID to it
//For a new process, we pre-populate a fake stack frame so the scheduler
//can "resume" it as if it was previously interrupted - popa reads the
//zeroed registers, then ret jumps to the entry point.
//Stack grows downward, so we push from the top of the stack[] array.
void pcb_create(void (*entry)()){
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (process_table[i].state == PROCESS_EMPTY) {
            process_table[i].id = i + 1;
            process_table[i].cr3 = (unsigned int)page_directory;
            for (int j = 0; j < 7; j++) {
                process_table[i].regs[j] = 0;
            }
            if (entry != 0) {
                //new process - build a fake stack frame for context_switch
                //popa pops: EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX (8 regs)
                //then ret pops the return address (entry point)
                unsigned int *stack_top = (unsigned int *)(process_table[i].esp);
                stack_top--;  
                *stack_top = 0;// EAX
                stack_top--;  
                *stack_top = 0;// ECX
                stack_top--;  
                *stack_top = 0;// EDX
                stack_top--;  
                *stack_top = 0;// EBX
                stack_top--;  
                *stack_top = 0;// ESP (ignored by popa)
                stack_top--;  
                *stack_top = 0;// EBP
                stack_top--;  
                *stack_top = 0;// ESI
                stack_top--;  
                *stack_top = 0;// EDI
                stack_top--;  
                *stack_top = (unsigned int)entry;  // return address = entry point
                
                //update ESP to point to the new stack top
                process_table[i].esp = (unsigned int)stack_top;
                process_table[i].eip = (unsigned int)entry;
                process_table[i].state = PROCESS_READY;
            }
            else {
                // entry == 0: register the current kernel thread as process 0
                // ESP will be saved properly on first context switch
                process_table[i].esp = (unsigned int)process_table[i].stack + 4096;
                process_table[i].eip = 0;
                process_table[i].state = PROCESS_RUNNING;
                current_process = i;
            }
        }
    }
    // silently fails if table is full — add error handling in future
}

//Initialises the process table at boot, marking all slots as empty.
//Must be called before pcb_create or the scheduler.
void pcb_init() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        process_table[i].state = PROCESS_EMPTY;
        process_table[i].id = 0;
    }
    process_count = 0;
    current_process = 0;
}