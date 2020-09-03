#include <igris/sync/syslock.h>
#include <mutex>
#include <cassert>

#include <igris/dprint.h>

#define NODTRACE 1
#include <igris/dtrace.h>

static std::recursive_mutex mtx;
//static std::mutex mtx;
static __thread int count = 0;

__BEGIN_DECLS

void system_lock() 
{
	DTRACE_ARGS(count);
	mtx.lock();
	
	if (count == 0) 
	{
		//save = tmpsave;
	}

	++count;
	assert(count < 10);
}

void system_unlock() 
{
	DTRACE_ARGS(count);
	--count;
	assert(count >= 0);

	mtx.unlock();
}

struct syslock_save_pair system_lock_save() 
{
	auto ret = syslock_save_pair{count, 0};
	assert(count != 0);

	while (count--) 
	{
		mtx.unlock();
	}

	return ret;
}

void system_lock_restore(struct syslock_save_pair save) 
{
	mtx.lock();

	count = save.count;
	int curcount = 1;

	while(curcount != count) 
	{
		curcount++;
		mtx.lock();
	}
}

void syslock_reset() 
{
	count = 0;
}

int syslock_counter() 
{
	return count;
}

void syslock_counter_set(int newcount) 
{
	count = newcount;
}


__END_DECLS