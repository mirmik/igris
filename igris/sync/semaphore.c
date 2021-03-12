#include <igris/sync/semaphore.h>

void semaphore_init(struct semaphore * sem, int val)
{
	sem->count = val;
	dlist_init(&sem->wait_list);
}

void semaphore_down(struct semaphore * sem)
{
	void * _;
	system_lock();

	while(1)
	{
		if (sem->count >= 1)
		{
			sem->count--;
			system_unlock();
			return; // return point is here
		}

		system_unlock();
		wait_current_schedee(&sem->wait_list, 0, &_);
		system_lock();
	}
}

/*int semaphore_down_trylock(struct semaphore * sem)
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
}*/

void semaphore_up(struct semaphore * sem)
{
	sem->count++;
	unwait_one(&sem->wait_list, 0);
}
