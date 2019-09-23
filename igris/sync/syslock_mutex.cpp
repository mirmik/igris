#include <igris/sync/syslock.h>
#include <mutex>
#include <cassert>

#include <igris/dprint.h>

#define NODTRACE 1
#include <igris/dtrace.h>

static std::recursive_mutex mtx;
//static std::mutex mtx;
static volatile int count = 0;


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
	assert(0);
	dprln("system_lock_save");
	auto ret = syslock_save_pair{count, 0};

	if (count) 
	{
		count = 0;
		mtx.unlock();
	}

	return ret;
}

void system_lock_restore(struct syslock_save_pair save) 
{
	assert(0);
	dprln("system_lock_restore");
	count = save.count;
	
	if (count) 
	{
		mtx.lock();
	}
}

__END_DECLS