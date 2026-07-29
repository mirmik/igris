#ifndef GENOS_DATASTRUCT_RING_HEAD_H
#define GENOS_DATASTRUCT_RING_HEAD_H

#include <igris/compiler.h>

struct ring_head
{
    unsigned int head;
    unsigned int tail;
    unsigned int size;
};

#define RING_HEAD_INIT(size)                                                   \
    {                                                                          \
        0, 0, size                                                             \
    }

__BEGIN_DECLS

static inline struct ring_head *ring_init(struct ring_head *r,
                                          unsigned int r_size)
{
    r->size = r_size;
    r->head = 0;
    r->tail = 0;
    return r;
}

static inline void ring_clean(struct ring_head *r)
{
    r->head = 0;
    r->tail = 0;
}

static inline void ring_fixup_head(struct ring_head *r)
{
    while (r->head >= r->size)
        r->head -= r->size;
}

static inline void ring_fixup_tail(struct ring_head *r)
{
    while (r->tail >= r->size)
        r->tail -= r->size;
}

static inline int ring_fixup_index(struct ring_head *r, int index)
{
    return index % r->size;
}

__ALWAYS_INLINE static inline int ring_empty(struct ring_head *r)
{
    return r->head == r->tail;
}

static inline int ring_full(struct ring_head *r)
{
    return r->head == (r->tail ? r->tail : r->size) - 1;
}

static inline unsigned int ring_avail(struct ring_head *r)
{
    return (r->head >= r->tail) ? r->head - r->tail
                                : r->size + r->head - r->tail;
}

static inline unsigned int ring_room(struct ring_head *r)
{
    return (r->head >= r->tail) ? r->size - 1 + (r->tail - r->head)
                                : (r->tail - r->head) - 1;
}

static inline struct ring_head *ring_move_head_one(struct ring_head *r)
{
    ++r->head;
    if (r->head == r->size)
        r->head = 0;
    return r;
}

static inline struct ring_head *ring_move_tail_one(struct ring_head *r)
{
    ++r->tail;
    if (r->tail == r->size)
        r->tail = 0;
    return r;
}

static inline struct ring_head *ring_move_head(struct ring_head *r,
                                               unsigned int bias)
{
    r->head += bias;
    ring_fixup_head(r);
    return r;
}

static inline struct ring_head *ring_move_tail(struct ring_head *r,
                                               unsigned int bias)
{
    r->tail += bias;
    ring_fixup_tail(r);
    return r;
}

static inline int ring_putc(struct ring_head *r, char *buffer, char c)
{
    if (ring_full(r))
        return 0;
    *(buffer + r->head) = c;
    ring_move_head_one(r);
    return 1;
}

/*
 * Extract one byte without reserving a byte value as an empty marker.
 * Returns 1 on success and 0 when the ring is empty.
 */
static inline int
ring_try_getc(struct ring_head *r, const char *buffer, char *result)
{
    if (ring_empty(r))
        return 0;
    *result = *(buffer + r->tail);
    ring_move_tail_one(r);
    return 1;
}

static inline int ring_getc(struct ring_head *r, const char *buffer)
{
    char c;
    if (!ring_try_getc(r, buffer, &c))
        return -1;
    /* Keep -1 distinct from every possible byte value. */
    return (unsigned char)c;
}

static inline int ring_read(struct ring_head *r,
                            const char *buffer,
                            char *data,
                            unsigned int size)
{
    int ret = 0;
    while (size-- && ring_try_getc(r, buffer, data))
    {
        data++;
        ret++;
    }
    return ret;
}

static inline int ring_write(struct ring_head *r,
                             char *buffer,
                             const char *data,
                             unsigned int size)
{
    int ret = 0;
    while (size--)
    {
        if (ring_putc(r, buffer, *data++) == 0)
        {
            return ret;
        };
        ret++;
    }
    return ret;
}

#define ring_for_each(n, r)                                                    \
    for (unsigned int n = (r)->tail; n != (r)->head; n = (n + 1) % (r)->size)

__END_DECLS

#endif
