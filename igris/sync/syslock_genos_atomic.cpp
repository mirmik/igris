#include <igris/sync/syslock.h>
#include <hal/irq.h>

static irqstate_t save;
static volatile int count = 0;

__BEGIN_DECLS

void system_lock() 
{
	irqstate_t tmpsave = irqs_save();
	
	if (count == 0) 
		save = tmpsave;

	++count;
}

void system_unlock() 
{
	--count;
	if (count == 0) irqs_restore(save);
}

void syslock_reset() 
{
	count = 0;
}

__END_DECLS