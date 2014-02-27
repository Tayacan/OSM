
#ifndef USER_SEMAPHORES
#define USER_SEMAPHORES

#include "kernel/semaphore.h"

typedef usr_sem_t void;

typedef struct 
{
  char const* name;
  int num_stuck;
  semaphore_t* sem;
} usr_sem_entry;

typedef struct
{
  int size;
  int max_length_name;
  semaphore_t* lock;
  struct usr_sem_entry* entries;
} usr_sem_contain;


void usr_sem_init(int max_semaphores, int max_name_length);

usr_sem_t* usr_sem_open(char const* name, int value);

int usr_sem_p(usr_sem_t* handle);

int usr_sem_v(usr_sem_t* handle);

int usr_sem_destory(usr_sem_t* handle);

#endif
