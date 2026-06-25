#ifndef PCB_H
#define PCB_H

#define MAX_PROCESS 16



typedef enum{
    PROCESS_EMPTY,
    PROCESS_READY,
    PROCESS_RUNNING
} process_state;

struct process {
    unsigned int id;
    unsigned int regs[7];
    unsigned int esp;
    unsigned int eip;
    unsigned int cr3;
    process_state state;
    unsigned char stack[4096];
};

extern struct process process_table[MAX_PROCESS];
extern int current_process;
extern int process_count;
void pcb_init();
void pcb_create(void (*entry)());
#endif