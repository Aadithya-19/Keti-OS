#include "string.h"

//compares two strings
int strcompare(const char *dest, const char *src) {
    while (*dest && *src && *dest == *src) {
        dest++;
        src++;
    }
    return *dest - *src;
}

void strcopy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

//conversion
void int_to_str(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char temp[12];
    int i = 0;
    while (n > 0) {
        temp[i++] = '0' + (n % 10);
        n /= 10;
    }
    // temp is in reverse; copy backwards into buf
    int j;
    for (j = 0; j < i; j++) {
        buf[j] = temp[i - 1 - j];
    }
    buf[j] = '\0';
}