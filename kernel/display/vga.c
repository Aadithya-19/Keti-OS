/*VGA memory starts at 0xB8000
Each character is 2 bytes - ASCII then color
Screen is 80 columns × 25 rows
Color 0x0F is white on black*/
#include "vga.h"
#include "drivers/ports.h"
#include "lib/string.h"
#include "drivers/timer.h"

int cursor_row = 0;
int cursor_col = 0;

void update_cursor(int row, int col) {
    unsigned short pos = row * 80 + col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

//so this function will be used to display anything string we want to print onto the OS for now
//old function jsut statically printed regardless of anything and overwrote, now we will implement cursor technique
void print_vga(const char *str){
    // int rows = 25;
    // int col = 80;
    
    char *out = (char *)0xB8000;
    int i=0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            cursor_col = 0;
            cursor_row++;
            i++;
            continue;
        }
        int index = (cursor_row*80 + cursor_col)*2;
        out[index] = str[i];
        out[index + 1] = 0x0F;
        i++;
        cursor_col++;
        if (cursor_col==80){
            cursor_col=0;
            cursor_row++;
            if (cursor_row >= 25) {
                scroll_vga();
            }
        }

    }
    update_cursor(cursor_row, cursor_col);
}

void print_char_vga(char c){
    char *out = (char *)0xB8000;
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        return;
    }
    int index = (cursor_row*80 + cursor_col)*2;
    out[index] = c;
    out[index + 1] = 0x0F;
    cursor_col++;
    if (cursor_col==80){
        cursor_col=0;
        cursor_row++;
        if (cursor_row >= 25) {
            scroll_vga();
        }
    }
    update_cursor(cursor_row, cursor_col);
}

void print_hex_vga(unsigned int n){
    print_vga("0x");
    for (int i = 28; i>=0; i-=4){
        //keeps digit between 0 to 15
        int digit = (n >> i) & 0xF;
        char c;
        if (digit<10){
            c = '0' + digit;
        }
        else{
            c = 'A' + (digit - 10);
        }
        print_char_vga(c);
    }
    update_cursor(cursor_row, cursor_col);
}


void delete_char_vga(){
    if (cursor_col > 0) {
        cursor_col--;
    }
    char *out = (char *)0xB8000;
    int index = (cursor_row*80 + cursor_col)*2;
    out[index] = ' ';
    out[index + 1] = 0x0F;
    if (cursor_col == 0) {
        cursor_row--;
        cursor_col = 79; //Now, being the smart person you are, you would have noticed that this will take you to the end of the line regardless of if you typed till the end of the line (so you are deleting nothing)
        //fix? i dont know \(*.*)/
        //ill come back to it
    } // a quick thought since i cant just keep pressing the cursor for each row, when you reach beginning of the line, a delete will take you up.
    update_cursor(cursor_row, cursor_col);
}

//self explanatory, clears it
void clear_vga(){
    char *out = (char *)0xB8000;
    int i = 0;
    //2000 because thats the total available characters (4000 bytes, cuz each is 2bytes)
    while (i < 2000) {
        out[i*2] = ' ';
        out[i*2 + 1] = 0x0F;
        i++;
    }
    cursor_row = 0;
    cursor_col = 0;
    update_cursor(cursor_row, cursor_col);
}


//time integration (fun)

void write_pos(int row, int col, const char *str){
    char *out = (char *)0xB8000;
    int i = 0;
     while (str[i] != '\0') {   
        int index = (row*80 + col + i)*2;
        out[index] = str[i];
        out[index + 1] = 0x0F;
        i++;

    }
}

void scroll_vga(){
    char *out = (char *)0xB8000;
    for (int row = 1; row < 25; row++){
        for (int col = 0; col < 80; col++){
            int src = (row * 80 + col) * 2;
            int dest = ((row-1) * 80 + col) * 2;
            out[dest] = out[src];
            out[dest+1] = out[src+1];
        }
    }

    for (int col = 0; col < 80; col++){
        int index = (24 * 80 + col) * 2;
        out[index] = ' ';
        out[index + 1] = 0x0F;
    } 
    cursor_row = 24;
}