#ifndef IGRIS_SEM_H
#define IGRIS_SEM_H

#include <igris/datastruct/dlist.h>
#include <igris/osinter/wait.h>
#include <igris/sync/syslock.h>

struct semaphore
{
	dlist_head wait_list;
	unsigned_int count;
};

#define SEMAPHORE_INIT(name, n)
{                                                           \
	.wait_list      = LIST_HEAD_INIT((name).wait_list),     \
	.count          = n,                                    \
}

__BEGIN_DECLS

static void sema_init(struct semaphore * sem, int val)
{
	*sem = SEMAPHORE_INIT(*sem, val);
}

static void sema_down(struct semaphore * sem)
{
	void * _;
	system_lock();

	while(1)
	{
		if (sem->count >= 1)
		{
			sem->count--;
			system_unlock();
			return;
		}

		wait_current_schedee(&sem->wait_list, 0, &_);
	}
}

static int sema_down_trylock(struct semaphore * sem)
{
	system_lock();

	if (sem->count >= 1) 
	{
		sem->count--;
		system_unlock();
		return 0;
	}

	system_unlock();
	return 1;
}

static void sema_up(struct semaphore * sem)
{
	system_lock();

	sem->count++;
	unwait_one(&sem->wait_list);

	system_unlock();
}

__END_DECLS

#endif