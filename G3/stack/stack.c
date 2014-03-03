#include "stack.h"
#include <stdlib.h>

#define LOCK pthread_mutex_lock
#define UNLOCK pthread_mutex_unlock

void stack_init(stack_t* stack)
{
  stack->top = NULL;
  pthread_mutex_init(&stack->lock,NULL);
}


int stack_empty(stack_t* stack)
{
  int result;
  LOCK(&stack->lock);
  result = stack->top == NULL;
  UNLOCK(&stack->lock);
  return result;
}

void* stack_top(stack_t* stack)
{
  void* result;
  LOCK(&stack->lock);
  if (stack->top == NULL)
  {
    UNLOCK(&stack->lock);
    return NULL;
  }
  result = stack->top->content;
  UNLOCK(&stack->lock);
  return result;
}

void* stack_pop(stack_t* stack)
{
  void* result;
  linked_entry *tmp;
  LOCK(&stack->lock);
  if (stack->top == NULL)
  {
    UNLOCK(&stack->lock);
    return NULL;
  }
  tmp = stack->top;
  stack->top = tmp->next;
  result = tmp->content;
  free(tmp);
  UNLOCK(&stack->lock);
  return result;
}

int stack_push(stack_t* stack, void* item)
{
  linked_entry* target = (linked_entry*) malloc(sizeof(linked_entry));
  if (target == NULL)
  {
    return 1;
  }
  target->content = item;
  LOCK(&stack->lock);
  target->next = stack->top;
  stack->top = target;
  UNLOCK(&stack->lock);
  return 0;
}
