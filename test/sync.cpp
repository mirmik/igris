#include <gtest/gtest.h>
#include <igris/sync/syslock.h>

TEST(sync, system_lock) 
{
	system_lock();
	system_unlock();
}
