#ifndef IGRIS_SEM_H
#define IGRIS_SEM_H

#if __has_include(<semaphore.h>)
#include <semaphore.h>
#else

#include <igris/datastruct/dlist.h>
#include <igris/osinter/wait.h>
#include <igris/sync/syslock.h>

typedef struct semaphore
{
    struct dlist_head wait_list;
    int count;
} sem_t;

#define SEMAPHORE_INIT(name, n)                                                \
    {                                                                          \
        .wait_list = DLIST_HEAD_INIT((name).wait_list), .count = n,            \
    }

#define SEMAPHORE(name, n) struct semaphore name = SEMAPHORE_INIT(name, n)

__BEGIN_DECLS

void sem_init(struct semaphore *sem, int val);

void sem_wait(struct semaphore *sem);

int sem_trywait(struct semaphore *sem);

void sem_post(struct semaphore *sem);

int sem_getvalue(struct semaphore *sem);

__END_DECLS

#endif
#endif
