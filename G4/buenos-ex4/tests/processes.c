#include "tests/lib.h"
#define PROCS 100

int main(void)
{
    int i;
    pid_t pids[PROCS];

    for(i = 0; i < PROCS; i++)
    {
        pids[i] = syscall_exec("[arkimedes]prog");
        printf("Started process %d\n", i);
    }

    for(i = 0; i < PROCS; i++)
    {
        syscall_join(pids[i]);
        printf("Joined process %d\n", i);
    }

    printf("Working as intended\n");

    //syscall_halt();
    return 0;
}
