/*VGA memory starts at 0xB8000
Each character is 2 bytes - ASCII then color
Screen is 80 columns × 25 rows
Color 0x0F is white on black*/

int cursor_row = 0;
int cursor_col = 0;

//so this function will be used to display anything string we want to print onto the OS for now
//old function jsut statically printed regardless of anything and overwrote, now we will implement cursor technique
void print_vga(char *str){
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
        }

    }
}


//self explanatory, clears it
void clear_vga(){
    char *out = (char *)0xB8000;
    int i = 0;
    //2000 because thats the total available characters (4000 bytes, cuz each is 2bytes)
    while (i<2000) {
        out[i*2] = ' ';
        out[i*2 + 1] = 0x0F;
        i++;
    }
}