#ifndef IGRIS_SEM_H
#define IGRIS_SEM_H

#include <igris/datastruct/dlist.h>
#include <igris/osinter/wait.h>
#include <igris/sync/syslock.h>

struct semaphore
{
	dlist_head wait_list;
	int count;
};

#define SEMAPHORE_INIT(name, n)                             \
{                                                           \
	.wait_list      = DLIST_HEAD_INIT((name).wait_list),    \
	.count          = n,                                    \
}

__BEGIN_DECLS

void semaphore_init(struct semaphore * sem, int val);

void semaphore_down(struct semaphore * sem);

int semaphore_down_trylock(struct semaphore * sem);

void semaphore_up(struct semaphore * sem);

__END_DECLS

#endif