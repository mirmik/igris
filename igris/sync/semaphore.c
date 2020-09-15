#include <igris/sync/semaphore.h>

static void semaphore_init(struct semaphore * sem, int val)
{
	*sem = SEMAPHORE_INIT(*sem, val);
}

static void semaphore_down(struct semaphore * sem)
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

static int semaphore_down_trylock(struct semaphore * sem)
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

static void semaphore_up(struct semaphore * sem)
{
	system_lock();

	sem->count++;
	unwait_one(&sem->wait_list, 0);

	system_unlock();
}
