#include "vfs.h"
#include "lib/string.h"

struct inode inode_table[MAX_FILES];

int vopen(const char *f){
    for (int i=0; i<MAX_FILES; i++){
        if (inode_table[i].type == I_EMPTY) continue;
        if (strcompare(inode_table[i].name, f) == 0){
            return i;
        }
    }
    return -1;
}

int vcreate(const char *f, int writable){
    for (int i=0; i<MAX_FILES; i++){
        if (inode_table[i].type == I_EMPTY) {
            strcopy(inode_table[i].name, f);
            inode_table[i].type = I_FILE;
            inode_table[i].size = 0;
            inode_table[i].writable = writable;
            return i;
        }
    }
    return -1;//max limit reached
}

int vread(int fd, char *buf, unsigned int len){
    if (fd < 0 || fd >= MAX_FILES) return -1;
    if (inode_table[fd].type == I_EMPTY) return -1;
    unsigned int to_copy = len;
    if (to_copy > inode_table[fd].size) {
        to_copy = inode_table[fd].size;
    }
    
    for (unsigned int i = 0; i < to_copy; i++) {
        buf[i] = inode_table[fd].data[i];
    }
    return to_copy;
}

int vwrite(int fd, const char *buf, unsigned int len){
    if (fd < 0 || fd >= MAX_FILES) return -1;
    if (inode_table[fd].type == I_EMPTY) return -1;
    if (inode_table[fd].writable == 0) return -1;

    unsigned int to_copy = len;
    if (to_copy > 4096) {
        to_copy = 4096;
    }
    
    for (unsigned int i = 0; i < to_copy; i++) {
        inode_table[fd].data[i] = buf[i];
    }
    inode_table[fd].size = to_copy;
    return to_copy;
}

int vclose(int fd){
    if (fd < 0 || fd >= MAX_FILES) return -1;
    if (inode_table[fd].type == I_EMPTY) return -1;
    return 0;
}

void vinit(){
    for (int i=0; i<MAX_FILES; i++){
        inode_table[i].type = I_EMPTY;
    }
}