#include "shell.h"
#include "display/vga.h"
#include "lib/string.h"
#include "filesystem/vfs.h"

int input_pos = 0;
char input_buffer[INPUT_SIZE];

extern struct inode inode_table[];
extern int tz_offset_minutes;
extern int use_12_hour;
extern int dst_offset;

char history[HISTORY_SIZE][INPUT_SIZE];

int history_count = 0; 
int history_pos = -1; 

struct command commands[] = {
    {"help", cmd_help, "Understand the available commands"},
    {"clear", cmd_clear, "Clears the window"},
    {"about", cmd_about, "Learn more about Keti OS"},
    {"echo", cmd_echo, "Print what you feel"},
    {"timezone", cmd_timezone, "Timezone command (eg. timezone EST)"},
    {"clock", cmd_clock, "Set clock format (12 or 24 hour), (eg. clock 12)"},
    {"dst", cmd_dst, "Set daylight savings time (dst on/off)"},
    {"ls", cmd_ls, "Shows all files"},
    {"creat", cmd_creat, "Creates a new file (not a typo)"},
    {"cat", cmd_cat, "Prints the content of a file"},
    {0, 0, 0}    // sentinel marking the end
};
void shell_handle_input(char c){
    if (c == '\n') {
        print_char_vga('\n');
        if (input_buffer[0] != '\0' && history_count < HISTORY_SIZE) {
            for (int i = 0; i < INPUT_SIZE; i++) {
                history[history_count][i] = input_buffer[i];
            }
            history_count++;
        }
        history_pos = -1;
        shell_execute();
        
        // properly clear the entire buffer
        for (int i = 0; i < INPUT_SIZE; i++) {
            input_buffer[i] = '\0';
        }
        input_pos = 0; 
        print_vga("keti> ");
    }
    else if (c == 0x08 || c == 127) {
        // backspace - remove last character
        if (input_pos > 0){
            input_pos--;
            input_buffer[input_pos] = '\0';
            delete_char_vga();
        }
    }
    else if (c >= 32 && c < 127) { //covers ' ' to DEL
        if (input_pos < INPUT_SIZE - 1){
            input_buffer[input_pos] = c;
            input_pos++;
            print_char_vga(c);
        }
    }
}

void shell_execute() {
    //make sure it isnt null
    if (input_buffer[0] == '\0') return;

    //arguments after command
    char *args = "";
    //parsing through the input
    for (int i = 0; input_buffer[i] != '\0'; i++) {
        if (input_buffer[i] == ' ') {
            input_buffer[i] = '\0';
            args = &input_buffer[i + 1];
            break;
        }
    }
    for (int i = 0; commands[i].name != 0; i++) {
        if (strcompare(input_buffer, commands[i].name) == 0) {
            commands[i].handler(args);
            return;
        }
    }
    
    print_vga("unknown command: ");
    print_vga(input_buffer);
    print_char_vga('\n');
}

void cmd_help(char *args) {
    print_vga("Available commands:\n");
    for (int i = 0; commands[i].name != 0; i++) {
        print_vga("  ");
        print_vga(commands[i].name);
        print_vga(" : ");
        print_vga(commands[i].descp);
        print_char_vga('\n');
    }
}

void cmd_ls(char *args) {
    for (int i=0; i<MAX_FILES; i++){
        if (inode_table[i].type != I_EMPTY){
            print_vga(inode_table[i].name);
            print_char_vga('\n');
        }
    }
}

void cmd_creat(char *args) {
    if (!strcompare(args, "")){
        print_vga("usage: creat <filename>\n");
        return;
    }
    int fd = vcreate(args, 1);
    if (fd == -1) {
        print_vga("could not create file (table full?)\n");
    } else {
        print_vga("file created\n");
    }

}

void cmd_cat(char *args){
    if (strcompare(args, "") == 0) {
        print_vga("usage: cat <filename>\n");
        return;
    }
    int fd = vopen(args);
    if (fd == -1) {
        print_vga("file not found\n");
        return;
    }
    
    char buf[4096];
    int bytes = vread(fd, buf, 4096);
    buf[bytes] = '\0';
    print_vga(buf);
    print_char_vga('\n');
}

void cmd_timezone(char *args) {
    if (strcompare(args, "UTC") == 0)  tz_offset_minutes = 0;
    else if (strcompare(args, "IST") == 0) tz_offset_minutes = 5 * 60 + 30;
    else if (strcompare(args, "EST") == 0)   tz_offset_minutes = -5 * 60;
    else if (strcompare(args, "PST") == 0)  tz_offset_minutes = -8 * 60;
    else if (strcompare(args, "CST") == 0)  tz_offset_minutes = -6 * 60;
    else if (strcompare(args, "SST") == 0)  tz_offset_minutes = -11 * 60;
    else if (strcompare(args, "CEST") == 0) tz_offset_minutes = 2 * 60;
    else {
        print_vga("Unrecognized. Timezones available are: \n");
        print_vga(" - UTC (Standard)\n");
        print_vga(" - IST (India: Bengaluru)\n");
        print_vga(" - EST (US: New York)\n");
        print_vga(" - PST (US: San Francisco)\n");
        print_vga(" - CST (US:Chicago)\n");
        print_vga(" - SST (Singapore)\n");
        print_vga(" - CEST (Europe: Germany)\n");
        return;
    }
    print_vga("Timezone updated\n");
}

void cmd_clock(char *args) {
    if (strcompare(args, "12") == 0) {
        use_12_hour = 1;
        print_vga("Switched to 12-hour clock\n");
    } else if (strcompare(args, "24") == 0) {
        use_12_hour = 0;
        print_vga("Switched to 24-hour clock\n");
    } else {
        print_vga("usage: clock [12|24]\n");
    }
}

void cmd_clear(char *args) {
    clear_vga();
}
void cmd_dst(char *args) {
    if (strcompare(args, "on") == 0) {
        dst_offset = 60;
        print_vga("DST enabled\n");
    } else if (strcompare(args, "off") == 0) {
        dst_offset = 0;
        print_vga("DST disabled\n");
    } else {
        print_vga("usage: dst [on|off]\n");
    }
}

void cmd_about(char *args) {
    print_vga("Keti OS v1.0\n");
    print_vga("By Aadithya Vasudevan\n\n");
    print_vga("A 32-bit x86 operating system built from scratch in C and\n");
    print_vga("assembly. Features its own bootloader handoff, memory\n");
    print_vga("management with paging and a kernel heap, interrupt-driven\n");
    print_vga("keyboard input, and an interactive shell.\n\n");
    print_vga("UPCOMING FEATURES: \n");
    
    print_vga(" - Shell Tetris\n");
    print_vga(" - Native Keti Language\n");
    print_vga(" - In-built model chat bot on shell\n");
}
    
void cmd_echo(char *args) {
    print_vga(args);
    print_char_vga('\n');
}

void shell_init() {
    print_vga("keti> ");
}

void shell_up(){
    if (history_count==0) return;
    if (history_pos == -1) {
        history_pos = history_count - 1; // start at most recent
    } else if (history_pos > 0) {
        history_pos--;
    }

    while (input_pos>0){
        delete_char_vga();
        input_pos--;
    }
    int i = 0;
    while (history[history_pos][i] != '\0') {
        input_buffer[i] = history[history_pos][i];
        print_char_vga(history[history_pos][i]);
        i++;
    }
    input_buffer[i] = '\0';
    input_pos = i;
}

void shell_down(){
    if (history_pos == -1) return;
    if (history_pos < history_count - 1) {
        history_pos++;

        while (input_pos > 0) {
            delete_char_vga();
            input_pos--;
        }
        int i = 0;
        while (history[history_pos][i] != '\0') {
            input_buffer[i] = history[history_pos][i];
            print_char_vga(history[history_pos][i]);
            i++;
        }
        input_buffer[i] = '\0';
        input_pos = i;
    } 
    else {
        history_pos = -1;
        while (input_pos > 0) {
            delete_char_vga();
            input_pos--;
        }
        input_buffer[0] = '\0';
    }
}