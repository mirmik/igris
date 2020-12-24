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
