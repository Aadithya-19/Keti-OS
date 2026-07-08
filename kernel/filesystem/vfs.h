#ifndef VFS_H
#define VFS_H
#define MAX_FILES 64


typedef enum {
    I_FILE,
    I_DIRECTORY,
    I_EMPTY //unused slot in the table
} I_TYPE;

struct inode {
    char name[64];
    char data[4096];
    I_TYPE type;
    unsigned int size;
    int writable; //0 = read only, 1 = read/write
};
extern struct inode inode_table[MAX_FILES];

int vopen(const char *f);
int vread(int fd, char *buf, unsigned int len);
int vwrite(int fd, const char *buf, unsigned int len);
int vclose(int fd);
int vcreate(const char *f, int writable);
void vinit();

#endif