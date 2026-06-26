#include "scheduler.h"
#include "pcb.h"
extern void context_switch();
extern struct process process_table[MAX_PROCESS];
extern int current_process;

//sets task to ready and allows them to get their 100ms time
//then it switches the context
void schedule(){
    if (process_count < 2) return;
    
    process_table[current_process].state = PROCESS_READY;
    int next_pro = (current_process + 1)%MAX_PROCESS;

    while (next_pro != current_process) {
        if (process_table[next_pro].state == PROCESS_READY) {
            break;
        }
        next_pro = (next_pro + 1) % MAX_PROCESS;
    }
    
    // update current and set to running
    current_process = next_pro;
    process_table[current_process].state = PROCESS_RUNNING;

    context_switch();
}

//sets the 100ms count time
void scheduler_tick() {
    if (process_count == 0) return;   
    static int tick_count = 0;
    tick_count++;
    if (tick_count >= 10) {
        tick_count = 0;
        schedule();
    }
}