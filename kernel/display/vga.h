#ifndef VGA_H
#define VGA_H

void print_vga(const char *str);
void print_char_vga(char c);
void print_hex_vga(unsigned int n);
void delete_char_vga(); 
void clear_vga();
void update_cursor(int row, int col);
void write_pos(int row, int col, const char *str);
void timer_handler();
void scroll_vga();

#endif