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
    intptr_t future;
    uint8_t type;
};

#define CTROBJ_DECLARE(name, type)                                             \
    {                                                                          \
        DLIST_HEAD_INIT(name.lnk), 0, type                                     \
    }

__BEGIN_DECLS

void ctrobj_init(struct ctrobj *obj, uint8_t type);
void ctrobj_deinit(struct ctrobj *obj);
intptr_t ctrobj_get_future(struct ctrobj *obj);
void ctrobj_set_future(struct ctrobj *obj, intptr_t future);

__END_DECLS

#endif
