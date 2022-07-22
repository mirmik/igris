#ifndef IGRIS_SCHEDEE_MANAGER_H
#define IGRIS_SCHEDEE_MANAGER_H

#include <algorithm>
#include <array>
#include <igris/container/dlist.h>
#include <igris/sync/syslock.h>

namespace igris
{
    class schedee_manager;
    class schedee
    {
        friend class schedee_manager;

    private:
        dlist_head lnk = DLIST_HEAD_INIT(lnk);
        uint8_t priority = 0;

    protected:
        virtual void execute() = 0;
        virtual void finalize() = 0;
        virtual void displace() = 0;
        virtual ~schedee() = default;
    };

    class schedee_manager
    {
        using schedee_list_t = igris::dlist<schedee, &schedee::lnk>;

        static constexpr int SCHEDEE_PRIORITY_TOTAL = 3;
        std::array<schedee_list_t, SCHEDEE_PRIORITY_TOTAL> run_list = {};
        schedee_list_t final_list = {};

    public:
        void exec()
        {
            system_lock();
            while (!final_list.empty())
            {
                auto &sch = final_list.front();
                final_list.unlink(sch);
                system_unlock();
                sch.finalize();
                system_lock();
            }
            system_unlock();

            system_lock();
            auto list_it =
                std::find_if(run_list.begin(), run_list.end(), [](auto &list) {
                    return !list.empty();
                });

            if (list_it == run_list.end())
            {
                system_unlock();
                return; // nothing to execute
            }

            auto &sch = list_it->front();
            list_it->move_back(sch);
            system_unlock();
            sch.execute();
            return;
        }

        void start(schedee &sch)
        {
            igris::syslock_guard lock;
            run_list[sch.priority].move_back(sch);
        }

        static void stop(schedee &sch)
        {
            igris::syslock_guard lock;
            schedee_list_t::unlink(sch);
        }

        void final(schedee &sch)
        {
            igris::syslock_guard lock;
            final_list.move_back(sch);
        }
    };
}

#endif