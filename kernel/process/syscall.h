#ifndef SYSCALL_H
#define SYSCALL_H

#define SYS_WRITE  1
#define SYS_READ   2
#define SYS_EXIT   3

void sys_write(const char *str, unsigned int len);
void sys_read();
void sys_exit();

void syscall_handler(unsigned int syscall_num, unsigned int arg1, unsigned int arg2);

#endif