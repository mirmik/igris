#include <igris/ctrobj/wait.h>
#include <igris/util/macro.h>
#include <igris/sync/syslock.h>

#include <mutex>
#include <condition_variable>

struct linux_waiter
{
	struct waiter w;
	std::mutex mut;
	std::condition_variable cvar;
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

	system_unlock();

	std::unique_lock<std::mutex> lock(waiter.mut);
	waiter.cvar.wait(lock);

	*future = waiter.w.ctr.future;
	return 0;
}

int unwait_schedee_waiter(struct waiter* w) 
{
	struct linux_waiter * waiter = mcast_out(w, struct linux_waiter, w);
	waiter->cvar.notify_one();
	return 0;
}