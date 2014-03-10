#include "tests/lib.h"

#define PROCESSES 10

int main(void)
{
    int i;
    pid_t pid[PROCESSES];

    for(i = 0; i < PROCESSES; i++)
    {
        pid[i] = syscall_exec("[arkimedes]malloc_child");
    }

    printf("Started %d processes\n", PROCESSES);

    for(i = 0; i < PROCESSES; i++)
    {
        syscall_join(pid[i]);
    }

    printf("Joined %d processes\n", PROCESSES);

    return 0;
}
