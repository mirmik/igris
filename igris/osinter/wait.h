#ifndef IGRIS_WAIT_H
#define IGRIS_WAIT_H

#include <igris/osinter/ctrobj.h>
#include <igris/compiler.h>

#include <sys/types.h>
#include <igris/util/types_extension.h>

#include <igris/datastruct/dlist.h>
#include <igris/datastruct/ring.h>

// TODO: Заменить dlist_head на wait_queue.
// 		Лучше использовать явно типизированную очередь.

#define WAIT_PRIORITY 1

struct waiter
{
	struct ctrobj ctr;
};

struct waiter_delegate
{
	struct ctrobj ctr;
	void (*func)(void*);
	void * obj;
};

__BEGIN_DECLS

static inline
void waiter_schedee_init(struct waiter * w) 
{
	ctrobj_init(&w->ctr, CTROBJ_WAITER_SCHEDEE);
}

static inline
void waiter_delegate_init(struct waiter_delegate * w, void (*func)(void*), void* obj) 
{
	ctrobj_init(&w->ctr, CTROBJ_WAITER_DELEGATE);
	w -> func = func;
	w -> obj = obj;
}

int wait_current_schedee(struct dlist_head * head, int priority, void ** future);
int unwait_schedee_waiter(struct waiter* w); 

int waiter_unwait(struct dlist_head * lnk, void* future);
void unwait_one(struct dlist_head * head, void* future);
void unwait_all(struct dlist_head * head, void* future);

ssize_t waited_ring_read(void* data, size_t size, int flags,
	struct ring_head* ring, char* ringbuf, struct dlist_head* wq);

__END_DECLS

#endif