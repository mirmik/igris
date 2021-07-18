#ifndef IGRIS_SEM_H
#define IGRIS_SEM_H

#include <igris/datastruct/dlist.h>
#include <igris/osinter/wait.h>
#include <igris/sync/syslock.h>

struct semaphore
{
    struct dlist_head wait_list;
    int count;
};

#define SEMAPHORE_INIT(name, n)                                                \
    {                                                                          \
        .wait_list = DLIST_HEAD_INIT((name).wait_list), .count = n,            \
    }

#define SEMAPHORE(name, n) struct semaphore name = SEMAPHORE_INIT(name, n)

__BEGIN_DECLS

void sem_init(struct semaphore *sem, int val);

void sem_down(struct semaphore *sem);

int sem_try_down(struct semaphore *sem);

void sem_up(struct semaphore *sem);

int sem_value(struct semaphore *sem);

__END_DECLS

#endif
