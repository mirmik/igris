#ifndef IGRIS_DATASTRUCT_BYTERING_H
#define IGRIS_DATASTRUCT_BYTERING_H

#include <igris/compiler.h>

struct bytering_head
{
    unsigned char *start;
    unsigned char *head;
    unsigned char *tail;
    unsigned char *end;
};

__BEGIN_DECLS

static inline void
bytering_init(struct bytering_head *r, void *buf, unsigned int size)
{
    r->start = (unsigned char *)buf;
    r->head = (unsigned char *)buf;
    r->tail = (unsigned char *)buf;
    r->end = (unsigned char *)buf + size;
}

static inline void __bytering_fixup(struct bytering_head *r,
                                    unsigned char **fixed)
{
    if (*fixed >= r->end)
        *fixed = r->start;
}

static inline int bytering_empty(struct bytering_head *r)
{
    return r->head == r->tail;
}

static inline int bytering_full(struct bytering_head *r)
{
    if (r->start == r->end)
        return 1;

    unsigned char *next = r->tail + 1;
    if (next >= r->end)
        next = r->start;

    return next == r->head;
}

static inline int bytering_pop_nocheck(struct bytering_head *r)
{
    unsigned char ret = *r->head++;
    __bytering_fixup(r, &r->head);
    return ret;
}

static inline int bytering_push_nocheck(struct bytering_head *r,
                                        unsigned char c)
{
    *r->tail++ = c;
    __bytering_fixup(r, &r->tail);
    return 0;
}

static inline int bytering_pop(struct bytering_head *r)
{
    if (bytering_empty(r))
        return -1;
    return bytering_pop_nocheck(r);
}

static inline int bytering_push(struct bytering_head *r, unsigned char c)
{
    if (bytering_full(r))
        return -1;
    bytering_push_nocheck(r, c);
    return 0;
}

__END_DECLS

#endif
