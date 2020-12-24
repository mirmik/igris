#ifndef IGRIS_SCHEDULER_STIMER_H
#define IGRIS_SCHEDULER_STIMER_H

struct ktimer_head 
{
	struct dlist_head;
	long start;
	long interval;
};

#endif
