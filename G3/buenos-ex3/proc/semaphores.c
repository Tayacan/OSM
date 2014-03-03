
#include "proc/semaphores.h"
#include "lib/libc.h"

semaphore_t *sem_table_lock;
usr_sem_entry sem_table[MAX_SEMAPHORES];

usr_sem_t* toHandle(int index)
{
  return (usr_sem_t*) (0xFFFFFFFF ^ index);
}

int toIndex(usr_sem_t* handle)
{
  return  (0xFFFFFFFF ^ (int) handle);
}

void init_user_sem_entry(usr_sem_entry *entry)
{
  entry->num_stuck = 0;
  entry->sem = NULL;
}

void usr_sem_init()
{
  int i;
  sem_table_lock = semaphore_create(1);
  for (i = 0; i < MAX_SEMAPHORES; i++)
  {
    init_user_sem_entry(&(sem_table[i]));
  }
}

usr_sem_t* usr_sem_open(char const* name, int value)
{
  int i, j;
  usr_sem_entry *target = NULL;
  usr_sem_entry *end_target = NULL;
  semaphore_P(sem_table_lock);
  j = 0;
  if (value >= 0)
  {
    for (i = 0; i < MAX_SEMAPHORES; i++)
    {
      target = &sem_table[i];
      if (target->sem == NULL)
      {
        if (end_target == NULL)
        {
          end_target = target;
          j = i;
        }
      }
      else
      {
        if (stringcmp(name, target->name) == 0)
        {
          semaphore_V(sem_table_lock);
          return NULL;
        }
      }
    }
    // copy string from userland space
    stringcopy(end_target->name, name, MAX_NAME_LENGTH);
    end_target->sem = semaphore_create(value);
    semaphore_V(sem_table_lock);
    return toHandle(j);
  }
  else
  {
    for (i = 0; i < MAX_SEMAPHORES; i++)
    {
      target = &sem_table[i];
      if (target->sem != NULL && stringcmp(target->name, name) == 0)
      {
        semaphore_V(sem_table_lock);
        return toHandle(i);
      }
    }
    semaphore_V(sem_table_lock);
    return NULL;
  }
}

int usr_sem_p(usr_sem_t* handle)
{
  if(handle == NULL)
  {
    return -2;
  }
  semaphore_P(sem_table_lock);
  usr_sem_entry target = sem_table[toIndex(handle)];
  if (target.sem == NULL)
  {
    semaphore_V(sem_table_lock);
    return -1;
  }
  target.num_stuck++;
  semaphore_V(sem_table_lock);
  semaphore_P(target.sem);
  semaphore_P(sem_table_lock);
  target.num_stuck--;
  semaphore_V(sem_table_lock);
  return 0;
}

int usr_sem_v(usr_sem_t* handle)
{
  if(handle == NULL)
  {
    return -2;
  }
  semaphore_P(sem_table_lock);
  usr_sem_entry target = sem_table[toIndex(handle)];
  if (target.sem == NULL)
  {
    semaphore_V(sem_table_lock);
    return -1;
  }
  semaphore_V(target.sem);
  semaphore_V(sem_table_lock);
  return 0;
}

int usr_sem_destory(usr_sem_t* handle)
{
  if(handle == NULL)
  {
    return -2;
  }
  semaphore_P(sem_table_lock);
  usr_sem_entry target = sem_table[toIndex(handle)];
  if (target.num_stuck == 0)
  {
    semaphore_destroy(target.sem);
    target.sem = NULL;
    semaphore_V(sem_table_lock);
    return 0;
  }
  else
  {
    semaphore_V(sem_table_lock);
    return -1;
  }
}

