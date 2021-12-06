#ifndef IGRIS_TIME_TIMER_MANAGER_H
#define IGRIS_TIME_TIMER_MANAGER_H

#include <igris/container/dlist.h>
#include <igris/event/delegate.h>
#include <igris/sync/syslock.h>
#include <igris/time/systime.h>

#include <functional>
#include <tuple>

namespace igris
{
    template <class Time_t, class Difftime_t = decltype(std::declval<time_t>() -
                                                        std::declval<time_t>())>
    class timer_spec
    {
    public:
        using time_t = Time_t;
        using difftime_t = Difftime_t;
    };

    template <typename TimeSpec> class timer_manager;

    template <typename TimeSpec> class timer_head
    {
    public:
        TimeSpec::time_t start;
        TimeSpec::difftime_t interval;

        TimeSpec::time_t final() { return start + interval; }
    };

    template <typename TimeSpec> class managed_timer_base : timer_head<TimeSpec>
    {
    public:
        dlist_head lnk = DLIST_HEAD_INIT(lnk);
        timer_manager<TimeSpec> *manager;

        void unplan() { dlist_del_init(&lnk); }

        virtual void execute() = 0;
    };

    template <typename TimeSpec, typename... Args>
    class timer_delegate : public managed_timer_base<TimeSpec>
    {
        igris::delegate<void, Args...> dlg;
        std::tuple<Args...> args;

    public:
        timer_delegate(igris::delegate<void, Args...> dlg, Args &&... args)
            : dlg(dlg), args(std::forward<Args>(args)...)
        {
        }

        void execute() override { std::apply(dlg, args); }
    };

    template <typename... Args>
    class timer : public timer_delegate<timer_spec<systime_t>, Args...>
    {
        using parent = timer_delegate<timer_spec<systime_t>, Args...>;

    public:
        timer(igris::delegate<void, Args...> dlg, Args &&... args)
            : parent(dlg, std::forward<Args>(args)...)
        {
        }
    };

    template <typename TimeSpec> class timer_manager
    {
        using timer = managed_timer_base<TimeSpec>;

        igris::dlist<timer, &timer::lnk> timer_list;

        igris::delegate<typename TimeSpec::time_t> gettime;

    public:
        timer_manager(igris::delegate<typename TimeSpec::time_t> gettime)
            : gettime(gettime){};

        void plan(timer &tim, TimeSpec::time_t start,
                  TimeSpec::difftime_t interval)
        {
            auto final = tim.final();

            system_lock();
            tim->unplan();

            auto it = std::find_if(timer_list.begin(), timer_list.end(),
                                   [](auto &tim) {
                                       auto it_final = tim.start + tim.interval;
                                       return final - it_final < 0;
                                   });

            timer_list.insert(it, tim);
            system_unlock();
        }

        void exec(TimeSpec::time_t cuttime)
        {
            system_lock();

            while (!dlist_empty(&ktimer_list))
            {
                ktimer_t *it =
                    dlist_first_entry(&ktimer_list, ktimer_t, ctr.lnk);
                system_unlock();

                if (ktimer_check(it, curtime))
                {
                    ktimer_execute(it);
                }
                else
                    return;

                system_lock();
            }

            system_unlock();
        }
    };
}

#endif