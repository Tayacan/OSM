#include "tests/lib.h"

int main(void)
{
    int i = 0;
    int n = 1 * 4096 - 8;

    heap_init();

    while(1)
    {
        printf("Allocating roughly 1 pages. Iteration: %d\n", i++);
        malloc(n);
    }

    syscall_halt();
    return 0;
}
