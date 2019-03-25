#include <gxx/syslock.h>
#include <hal/irq.h>

static irqstate_t save;
static int count = 0;

__BEGIN_DECLS

void system_lock() {
	if (count == 0) save = irqs_save();
	++count;
}

void system_unlock() {
	--count;
	if (count == 0) irqs_restore(save);
}

__END_DECLS