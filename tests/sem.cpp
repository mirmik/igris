#include <doctest/doctest.h>
#include <igris/sync/semaphore.h>

TEST_CASE("semaphore") 
{
	SEMAPHORE(sem, 1);

	sem_try_down(&sem);
	CHECK_EQ(sem_value(&sem), 0);

	sem_up(&sem);
	CHECK_EQ(sem_value(&sem), 1);

	sem_up(&sem);
	CHECK_EQ(sem_value(&sem), 2);
}