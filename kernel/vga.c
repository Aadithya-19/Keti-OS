/*VGA memory starts at 0xB8000
Each character is 2 bytes - ASCII then color
Screen is 80 columns × 25 rows
Color 0x0F is white on black*/

//so this function will be used to display anything string we want to print onto the OS for now
void print_vga(char *str){
    // int rows = 25;
    // int col = 80;

    char *out = (char *)0xB8000;
    int i = 0;
    while (str[i] != '\0') {
        out[i*2] = str[i];
        out[i*2 + 1] = 0x0F;
        i++;
    }
}


//self explanatory, clears it
void clear_vga(){
    char *out = (char *)0xB8000;
    int i = 0;
    while (i<2000) {
        out[i*2] = ' ';
        out[i*2 + 1] = 0x0F;
        i++;
    }
}