#ifndef IGRIS_DATASTRUCT_TIMER_H
#define IGRIS_DATASTRUCT_TIMER_H

struct timer_head 
{
	unsigned long start;
	long          interval;
};

__BEGIN_DECLS

static inline void timer_init(
	struct timer_head * tim,
	unsigned long start,
	long          interval
) {
	tim->start = start;
	tim->interval = interval;
}

static inline void timer_swift(struct timer_head * tim) 
{
	tim->start = tim->start + tim->interval;
}

static inline unsigned long timer_finish(struct timer_head * tim)
{
	return tim->start + tim->interval;
}

static inline int timer_check(struct timer_head * tim, 
	unsigned long curtime)
{
	return curtime - tim->start >= tim->interval;
}

__END_DECLS

#endif