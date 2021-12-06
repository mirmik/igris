#ifndef IGRIS_TIME_TIMER_MANAGER_H
#define IGRIS_TIME_TIMER_MANAGER_H

#include <igris/container/dlist.h>

namespace igris
{
    template <typename Timer> class timer_manager;

    class timer
    {
        dlist_head lnk = DLIST_HEAD_INIT(lnk);

        void unplan() { dlist_del_init(&lnk); }

        void execute();
    };

    template <typename Timer, dlist_head Timer::*lnk>;
    class timer_manager
    {
        igris::dlist<Timer, lnk> timer_list;

    public:
        void plan(timer &tim) {}
    };
}

#endif