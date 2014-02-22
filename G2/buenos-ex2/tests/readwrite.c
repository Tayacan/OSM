#include "tests/lib.h"

int prompt(char *p, char *buf, int len) {
    puts(p);
    return readline(buf, len);
}

int main(void) {
    char buffer[64];

    //syscall_exec("[arkimedes]useless");

    puts("Hello, World!\n");
    puts("Type q or quit to get out.\n");

    while(1) {
        prompt("echo> ", buffer, 63);
        if(strcmp(buffer, "q") == 0 || strcmp(buffer, "quit") == 0) {
            break;
        } else {
            puts(buffer);
            puts("\n");
        }
    }

    syscall_halt();

    return 0;
}
