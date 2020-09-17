#include <igris/osinter/wait.h>
#include <igris/util/macro.h>
#include <igris/sync/syslock.h>
#include <igris/syncxx/event.h>

struct linux_waiter
{
	struct waiter w;
	igris::event event;
};

int wait_current_schedee(struct dlist_head * head, int priority, void ** future) 
{
	struct linux_waiter waiter;

	waiter_schedee_init(&waiter.w);

	system_lock();

	if (priority) 
		dlist_move(&waiter.w.ctr.lnk, head);
	else 
		dlist_move_tail(&waiter.w.ctr.lnk, head);

	auto save = system_lock_save();
	waiter.event.wait();
	system_lock_restore(save);

	system_unlock();

	*future = waiter.w.ctr.future;
	return 0;
}

int unwait_schedee_waiter(struct waiter* w) 
{
	struct linux_waiter * waiter = mcast_out(w, struct linux_waiter, w);
	waiter->event.signal();

	return 0;
}