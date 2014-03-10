#include "tests/lib.h"

#define ALLOC_SIZE_0 47
#define ALLOC_SIZE_1 60
#define ALLOC_NUM 50

int main(void)
{
    int i;

    void* foo[ALLOC_NUM];

    heap_init();

    for(i = 0; i < ALLOC_NUM; i++)
    {
        foo[i] = malloc(ALLOC_SIZE_0);
    }

    printf("Allocated %d blocks of %d bytes...", ALLOC_NUM, ALLOC_SIZE_0);

    for(i = 0; i < ALLOC_NUM; i++)
    {
        free(foo[i]);
    }

    printf(" and freed them.\n");

    for(i = 0; i < ALLOC_NUM; i++)
    {
        foo[i] = malloc(ALLOC_SIZE_1);
    }

    printf("Allocated %d blocks of %d bytes...", ALLOC_NUM, ALLOC_SIZE_1);

    for(i = 0; i < ALLOC_NUM; i++)
    {
        free(foo[i]);
    }

    printf(" and freed them.\n");

    return 0;
}
