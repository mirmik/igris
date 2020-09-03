#include <igris/osinter/wait.h>

#include <igris/util/macro.h>
#include <igris/sync/syslock.h>

#include <igris/datastruct/dlist.h>
#include <igris/datastruct/ring.h>
#include <igris/defs/io.h>
#include <igris/defs/schedee.h>

int waiter_unwait(struct dlist_head * lnk, void* future)
{
	struct ctrobj * ctr = mcast_out(lnk, struct ctrobj, lnk);
	ctr->future = future;

	switch (ctr->type)
	{
		case CTROBJ_WAITER_SCHEDEE:
			{
				struct waiter * w = mcast_out(ctr, struct waiter, ctr);
				unwait_schedee_waiter(w);
			}
			return 0;

		case CTROBJ_WAITER_DELEGATE:
			{
				struct waiter_delegate * w = mcast_out(ctr, struct waiter_delegate, ctr);
				w->func(w->obj);
			}
			return 0;

		default:
			return -1;
	}
}

void unwait_one(struct dlist_head * head, void * future)
{
	struct dlist_head * it;

	system_lock();

	if (dlist_empty(head))
	{
		system_unlock();
		return;
	}

	it = head->next;
	dlist_del_init(it);
	waiter_unwait(it, future);

	system_unlock();
}

void unwait_all(struct dlist_head * head, void* future)
{
	struct dlist_head * it;

	system_lock();

	while (!dlist_empty(head))
	{
		it = head->next;
		dlist_del_init(it);
		waiter_unwait(it, future);
	}

	system_unlock();
}

ssize_t waited_ring_read(void* data, size_t size, int flags,
                         struct ring_head* rxring,
                         char* rxbuffer,
                         struct dlist_head* rxwait)
{
	int ret;

	system_lock();

	while (ring_empty(rxring))
	{

		if (flags & IO_NOBLOCK)
		{
			system_unlock();
			return 0;
		}

		if (wait_current_schedee(rxwait, 0, NULL) == SCHEDEE_DISPLACE_VIRTUAL)
		{
			system_unlock();
			return 0;
		}
	}

	system_unlock();

	if (flags & IO_ONLYWAIT)
		return 0;

	system_lock();
	ret = ring_read(rxring, rxbuffer, (char*)data, size);
	system_unlock();

	return ret;
}