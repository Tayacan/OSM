#include "tests/lib.h"

int main(void)
{
    char *foo = malloc(10 * sizeof(char));

    free(foo);

    return 0;
}
