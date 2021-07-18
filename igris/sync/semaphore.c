#include <igris/sync/semaphore.h>

void sem_init(struct semaphore *sem, int val)
{
    sem->count = val;
    dlist_init(&sem->wait_list);
}

void sem_down(struct semaphore *sem)
{
    void *_;
    system_lock();

    while (1)
    {
        if (sem->count > 0)
        {
            --sem->count;
            system_unlock();
            break;
        }

        system_unlock();
        wait_current_schedee(&sem->wait_list, 0, &_);
        system_lock();
    }
}

int sem_try_down(struct semaphore *sem)
{
    int status = -1;

    system_lock();

    if (sem->count > 0)
    {
        --sem->count;
        status = 0;
    }

    system_unlock();
    return status;
}

void sem_up(struct semaphore *sem)
{
    system_lock();

    ++sem->count;
    if (!dlist_empty(&sem->wait_list))
        unwait_one(&sem->wait_list, 0);

    system_unlock();
}

int sem_value(struct semaphore *sem)
{
    int count;

    system_lock();
    count = sem->count;
    system_unlock();

    return count;
}
