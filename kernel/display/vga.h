
#ifndef VGA_H
#define VGA_H
 
extern int cursor_row;
extern int cursor_col;
 
void print_vga(const char *str);
void print_char_vga(char c);
void print_hex_vga(unsigned int n);
void delete_char_vga();
void clear_vga();
void write_pos(int row, int col, const char *str);
 
void render();
void scrollback_up();
void scrollback_down();
 
#endif
 