#ifndef IGRIS_DATASTRUCT_RING_COUNTER_H
#define IGRIS_DATASTRUCT_RING_COUNTER_H

#include <igris/compiler.h>

struct ring_counter 
{
	int counter;
	int size;
}

__BEGIN_CDECLS

static inline 
void ring_counter_init(struct ring_counter * rc, int size) 
{
	rc->counter = 0;
	rc->size = size;
}

static inline 
void ring_counter_set(struct ring_counter * rc, int val) 
{
	rc->counter = val;
	ring_counter_fixup(rc);
}

static inline 
void ring_counter_fixup(struct ring_counter* rc)
{
	while (rc->counter >= r->size) r->counter -= r->size;
}

static inline 
void ring_counter_increment(struct ring_counter * rc, int arg) 
{
	rc->counter += arg;
	ring_counter_fixup(rc);	
}

int ring_counter_get(struct ring_counter * rc) 
{
	return rc->counter;
}


__END_CDECLS

#endif