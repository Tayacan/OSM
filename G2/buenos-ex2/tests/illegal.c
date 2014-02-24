#include "tests/lib.h"

int main(void)
{
  printf("trying to join with 0");
  syscall_join(0);
  printf("joined with process 0");
  return 0;
}
