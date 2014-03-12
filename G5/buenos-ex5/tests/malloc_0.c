#include "tests/lib.h"

int main(void)
{
    heap_init();
    char *foo = malloc(10 * sizeof(char));

    foo = "Foo\n";
    printf(foo);

    free(foo);
    printf("freeeee!\n");

    syscall_halt();
    return 0;
}
