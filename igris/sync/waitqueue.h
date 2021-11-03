#ifndef IGRIS_SYNC_WAITQUEUE_H
#define IGRIS_SYNC_WAITQUEUE_H

#include <igris/compiler.h>
#include <igris/datastruct/dlist.h>

typedef struct waitqueue
{
    struct dlist_head lst;
} waitqueue_t;

__BEGIN_DECLS

int waitqueue_unwait_one(struct waitqueue *queue);
int waitqueue_wait(struct waitqueue *queue);
int waitqueue_no_displace_wait(struct waitqueue *queue);

__END_DECLS

#define WAITQUEUE_INIT(queue)                                                  \
    {                                                                          \
        DLIST_HEAD_INIT(queue.lst)                                             \
    }

#endif
