#include <igris/sync/syslock.h>
#include <hal/irq.h>

static irqstate_t save;
static volatile int count = 0;

__BEGIN_DECLS

void system_lock() 
{
	if (is_interrupt_context()) 
		return;

	if (count == 0) 
		save = irqs_save();
	++count;
}

void system_unlock() 
{
	if (is_interrupt_context()) 
		return;

	--count;
	if (count == 0) irqs_restore(save);
}

void syslock_reset() 
{
	count = 0;
}

__END_DECLS