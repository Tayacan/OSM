#include "tests/lib.h"
#include "tests/libio.h"

int main(void) {
    char buffer[64];

    write("Hello, World!\n");
    write("Type q or quit to get out.\n");

    while(1) {
        prompt("echo> ", buffer, 63);
        if(strcmp(buffer, "q\n") || strcmp(buffer, "quit\n")) {
            break;
        } else {
            write(buffer);
        }
    }

    syscall_halt();

    return 0;
}
