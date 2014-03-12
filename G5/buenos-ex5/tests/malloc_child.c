#include "tests/lib.h"

int main(void)
{
    int *foo;
    int i;

    heap_init();

    foo = malloc(sizeof(int) * 1030);

    foo[0] = 0;
    for(i = 1; i < 10; i++)
    {
        foo[i] = foo[i-1] + i;
    }

    printf("foo[7] = %d\n", foo[7]);

    syscall_exit(0);
    return 0;
}
