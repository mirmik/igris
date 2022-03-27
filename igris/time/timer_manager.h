#ifndef IGRIS_TIME_TIMER_MANAGER_H
#define IGRIS_TIME_TIMER_MANAGER_H

#include <igris/container/dlist.h>
#include <igris/event/delegate.h>
#include <igris/sync/syslock.h>
#include <igris/time/systime.h>

#include <algorithm>
#include <functional>
#include <tuple>

namespace igris
{
    template <class Time_t, class Difftime_t = decltype(std::declval<Time_t>() -
                                                        std::declval<Time_t>())>
    class timer_spec
    {
    public:
        using time_t = Time_t;
        using difftime_t = Difftime_t;
    };

    template <typename TimeSpec> class timer_manager;

    template <typename TimeSpec> class timer_head
    {
        using time_t = typename TimeSpec::time_t;
        using difftime_t = typename TimeSpec::difftime_t;

    public:
        time_t start=0;
        difftime_t interval=0;

        time_t finish() { return start + interval; }
        bool check(time_t curtime) { return curtime - start >= interval; }
        void set_start(time_t t) { start = t; }
        void set_interval(difftime_t t) { interval = t; }
        void shift() { start += interval; }
        virtual ~timer_head() = default;
    };

    template <typename TimeSpec>
    class managed_timer_base : public timer_head<TimeSpec>
    {
    public:
        dlist_head lnk = DLIST_HEAD_INIT(lnk);
        timer_manager<TimeSpec> *manager = nullptr;

        bool is_planned() { return lnk.next != &lnk && &lnk != lnk.prev; }
        void unplan() { dlist_del_init(&lnk); }

        virtual void execute() = 0;
        virtual ~managed_timer_base() = default;
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
        using time_t = typename TimeSpec::time_t;
        using difftime_t = typename TimeSpec::difftime_t;
        using timer = managed_timer_base<TimeSpec>;

        igris::dlist<timer, &timer::lnk> timer_list = {};
        igris::delegate<time_t> gettime = {};

    public:
        timer_manager(igris::delegate<time_t> gettime) : gettime(gettime){};

        void plan(timer &tim)
        {
            auto final = tim.finish();

            system_lock();
            tim.unplan();

            auto it = std::find_if(timer_list.begin(), timer_list.end(),
                                   [final](auto &tim) {
                                       auto it_final = tim.start + tim.interval;
                                       return final - it_final < 0;
                                   });

            timer_list.move_prev(tim, it);
            system_unlock();
        }

        void plan(timer &tim, time_t start, difftime_t interval)
        {
            tim.set_start(start);
            tim.set_interval(interval);
            plan(tim);
        }

        void exec(time_t curtime)
        {
            system_lock();

            while (!timer_list.empty())
            {
                timer &tim = timer_list.first();
                system_unlock();

                if (tim.check(curtime))
                {
                    tim.execute();
                    auto linked = tim.is_planned();
                    if (linked) 
                    {
                        tim.unplan();
                        tim.shift();
                        plan(tim);
                    }
                }
                else
                    return;

                system_lock();
            }

            system_unlock();
        }

        bool empty() const { return timer_list.empty(); }

        difftime_t minimal_interval(time_t curtime)
        {
            return timer_list.first().finish() - curtime;
        }
    };
}

#endif
