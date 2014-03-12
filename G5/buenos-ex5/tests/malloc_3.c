#include "tests/lib.h"

int main(void)
{
    char *foo;
    int i;
    int n = 10 * 4096;
    heap_init();

    foo = (char*)malloc(n);
    printf("Allocated %d bytes\n", n);
    for(i = 0; i < n; i++)
    {
        foo[i] = 'a';
    }
    printf("Wrote a lot of a's.\n");

    syscall_halt();
    return 0;
}
