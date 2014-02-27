
#include "proc/semaphores.h"
#include "kernel/kmalloc.h"

usr_sem_contain* SEMAPHORES;
#define SEMS SEMAPHORES

//#define MAX_NAME_LENGTH 20


usr_sem_t* toHandle(int index)
{
  return (usr_sem_t*) (0xFFFFFFFF ^ index);
}

int toIndex(usr_sem_t* handle)
{
  return  (0xFFFFFFFF ^ (int) handle);
}



int strcmp(char const* a, char const* b)
{
  int i;
  int max = SEMS->max_length_name;
  for(i = 0; i < max; i++) {
        if (a[i] != b[i])
        {
          return 0;
        }
        if (a[i] == '\0')
        {
          return 1;
        }
 }
 return 1;
}

void init_user_sem_entry(usr_sem_entry *entry, int max_name_length)
{
  entry->name = kmalloc(sizeof(char) * max_name_length);
  entry->num_stuck = 0;
  entry->sem = NULL;
}

void usr_sem_init(int max_semaphores, int max_name_length)
{
  int i;
  SEMS = (usr_sem_contain*) kmalloc(sizeof(usr_sem_contain*));
  SEMS->size = max_semaphores;
  SEMS->max_length_name = max_name_length;
  SEMS->lock = semaphore_create(1);
  SEMS->entries = (usr_sem_entry*) kmalloc(sizeof(usr_sem_entry) * max_semaphores);
  for (i = 0; i < max_semaphores; i++)
  {
    init_user_sem_entry(&(SEMS->entries[i]), max_name_length);
  }
}

usr_sem_t* usr_sem_open(char const* name, int value)
{
  int i, j;
  usr_sem_entry target;
  usr_sem_entry *end_target = NULL;
  semaphore_P(SEMS->lock);
  if (value >= 0)
  {
    for (i = 0; i < SEMS->size; i++)
    {
      target = SEMS->entries[i];
      if (target.sem == NULL)
      {
        if (end_target == NULL)
        {
          end_target = &target;
        }
      }
      else
      {
        if (strcmp(name, target.name))
        {
          semaphore_V(SEMS->lock);
          return NULL;
        }
      }
    }
    // copy string from userland space
    for (j = 0; j < SEMS->max_length_name; j++)
    {
      end_target->name[j] = name[j];
    }
    end_target->sem = semaphore_create(value);
    semaphore_V(SEMS->lock);
    return toHandle(i);
  }
  else
  {
    for (i = 0; i < SEMS->size; i++)
    {
      target = SEMS->entries[i];
      if (target.sem != NULL && strcmp(target.name, name))
      {
        semaphore_V(SEMS->lock);
        return toHandle(i);
      }
    }
    semaphore_V(SEMS->lock);
    return NULL;
  }
}

int usr_sem_p(usr_sem_t* handle)
{
  semaphore_P(SEMS->lock);
  usr_sem_entry target = SEMS->entries[toIndex(handle)];
  if (target.sem == NULL)
  {
    semaphore_V(SEMS->lock);
    return -1;
  }
  target.num_stuck++;
  semaphore_V(SEMS->lock);
  semaphore_P(target.sem);
  semaphore_P(SEMS->lock);
  target.num_stuck--;
  semaphore_V(SEMS->lock);
  return 0;
}

int usr_sem_v(usr_sem_t* handle)
{
  semaphore_P(SEMS->lock);
  usr_sem_entry target = SEMS->entries[toIndex(handle)];
  if (target.sem == NULL)
  {
    semaphore_V(SEMS->lock);
    return -1;
  }
  semaphore_V(target.sem);
  semaphore_V(SEMS->lock);
  return 0;
}

int usr_sem_destory(usr_sem_t* handle)
{
  semaphore_P(SEMS->lock);
  usr_sem_entry target = SEMS->entries[toIndex(handle)];
  if (target.num_stuck == 0)
  {
    semaphore_destroy(target.sem);
    target.sem = NULL;
    semaphore_V(SEMS->lock);
    return 0;
  }
  else
  {
    semaphore_V(SEMS->lock);
    return -1;
  }
}

