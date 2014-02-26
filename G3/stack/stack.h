#include <pthread.h>

typedef struct ll
{
  void *content;
  struct ll *next;
} linked_entry;

typedef struct
{
  linked_entry *top;
  pthread_mutex_t lock;
} stack_t;

void stack_init(stack_t* stack);

int stack_empty(stack_t* stack);

void* stack_top(stack_t* stack);

void* stack_pop(stack_t* stack);

int stack_push(stack_t* stack, void* item);
