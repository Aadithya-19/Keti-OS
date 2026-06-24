#ifndef TIMER_H
#define TIMER_H

void timer_init();
void timer_handler();
unsigned char read_rtc(unsigned char reg);
unsigned char bcd_to_bin(unsigned char val);

#endif