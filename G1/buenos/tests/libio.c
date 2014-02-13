#include "tests/libio.h"
#include "tests/lib.h"

int readline(char *buffer, int length) {
    int read_bytes = 0;
    int read_tmp;

    while((read_tmp = syscall_read(0, buffer++, 1)) && read_bytes < length) {
        if(buffer[-1] == '\n' || buffer[-1] == '\r') {
            buffer[-1] = '\n';
            read_bytes += read_tmp;
            syscall_write(1, "\n", 1);
            break;
        } else {
            syscall_write(1, buffer - 1, 1);
            read_bytes += read_tmp;
        }
    }
    *buffer = '\0';
    return read_bytes;
}

int strlength(char *buffer) {
    int len = 0;

    while(buffer[len]) {
        len++;
    }

    return len;
}

bool strcmp(char* b1, char* b2) {
    while(*b1 != '\0' && *b2 != '\0') {
        if(*b1 != *b2) {
            return false;
        }
        b1++;
        b2++;
    }
    return *b1 == *b2;
}

int prompt(char *prompt, char *buffer, int length) {
    write(prompt);
    return readline(buffer, length);
}

int write(char *buffer) {
    return syscall_write(1, buffer, strlength(buffer));
}
