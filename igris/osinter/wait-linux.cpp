#include <igris/osinter/wait.h>
#include <igris/sync/syslock.h>
#include <igris/syncxx/event.h>
#include <igris/util/macro.h>

struct linux_waiter
{
    waiter w;
    igris::event event = {};
};

int wait_current_schedee(igris::dlist_base *head, int priority, void **future)
{
    linux_waiter waiter;

    system_lock();

    if (priority)
        head->move_front(waiter.w.lnk);
    else
        head->move_back(waiter.w.lnk);
    system_unlock();

    // auto save = system_lock_save();
    waiter.event.wait();
    // system_lock_restore(save);

    *future = (void *)waiter.w.future;
    return 0;
}

int unwait_schedee_waiter(waiter *w)
{
    linux_waiter *waiter = mcast_out(w, struct linux_waiter, w);
    waiter->event.signal();

    return 0;
}
