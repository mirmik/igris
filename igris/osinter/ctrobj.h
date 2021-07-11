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

struct ctrobj
{
    struct dlist_head lnk;
    void *future;
    uint8_t type;
};

#define CTROBJ_DECLARE(name, type)                                             \
    {                                                                          \
        DLIST_HEAD_INIT(name.lnk), NULL, type                                  \
    }

__BEGIN_DECLS

static inline void ctrobj_init(struct ctrobj *obj, uint8_t type)
{
    obj->type = type;
    obj->future = NULL;
    dlist_init(&obj->lnk);
}

static inline void ctrobj_deinit(struct ctrobj *obj)
{
    dlist_del_init(&obj->lnk);
}

__END_DECLS

#endif
