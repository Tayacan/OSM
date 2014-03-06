#include "tests/lib.h"

int main(void)
{
    char foo[100];
    foo[99] = 'a';
    //printf("Hello, World!\n");

    syscall_exit(0);
    return 0;
}
