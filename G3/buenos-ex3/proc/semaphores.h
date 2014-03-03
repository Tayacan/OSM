
#ifndef USER_SEMAPHORES
#define USER_SEMAPHORES

#include "kernel/semaphore.h"

#define MAX_SEMAPHORES 50
#define MAX_NAME_LENGTH 30

typedef void usr_sem_t;

typedef struct 
{
  char name[MAX_NAME_LENGTH];
  int num_stuck;
  semaphore_t* sem;
} usr_sem_entry;

void usr_sem_init();

usr_sem_t* usr_sem_open(char const* name, int value);

int usr_sem_p(usr_sem_t* handle);

int usr_sem_v(usr_sem_t* handle);

int usr_sem_destroy(usr_sem_t* handle);

#endif
