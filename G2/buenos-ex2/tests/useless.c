#include "tests/lib.h"

int main(void) {
    puts("I'm useless.\n");
    syscall_exit(0);
    return 0;
}
