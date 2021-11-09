#ifndef IGRIS_SEM_H
#define IGRIS_SEM_H

#if __has_include(<asm_igris/semaphore.h>)
#include <asm_igris/semaphore.h>
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

void sem_init(struct semaphore *sem, int shared, int val);

int sem_wait(struct semaphore *sem);

int sem_trywait(struct semaphore *sem);

int sem_post(struct semaphore *sem);

int sem_getvalue(struct semaphore *sem, int *val);

int sem_destroy(struct semaphore *sem);

__END_DECLS

#endif
#endif
