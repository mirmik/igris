#ifndef IGRIS_CTROBJ_H
#define IGRIS_CTROBJ_H

// Объект синхронизации для построения очередей, корутин и прочей гадости.
// Живёт в памяти процесса или его структуре и подключается
// в очереди ожидания (см. файл wait.h)

#include <igris/datastruct/dlist.h>
#include <stdint.h>

#define CTROBJ_SCHEDEE_LIST 0
#define CTROBJ_WAITER_SCHEDEE 1
#define CTROBJ_WAITER_DELEGATE 2
#define CTROBJ_KTIMER_SCHEDEE 3
#define CTROBJ_KTIMER_DELEGATE 4

class ctrobj
{
public:
    struct dlist_head lnk = DLIST_HEAD_INIT(lnk);
    void *future = nullptr;
    uint8_t type = 0;

public:
    ctrobj() = default;
    ctrobj(uint8_t type) : type(type) {}

    void init(uint8_t type)
    {
        this->type = type;
        dlist_init(&lnk);
    }

    void deinit()
    {
        dlist_del(&lnk);
    }

    void *get_future()
    {
        return future;
    }
};

#endif
