#include <igris/sync/syslock.h>
#include <hal/irq.h>

#include <assert.h>

static irqstate_t save;
static volatile int count = 0;

__BEGIN_DECLS

void system_lock() 
{
	irqstate_t tmpsave = irqs_save();
	
	if (count == 0) 
		save = tmpsave;

	++count;
	assert(count < 10);
}

void system_unlock() 
{
	--count;
	assert(count >= 0);

	if (count == 0) irqs_restore(save);
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
