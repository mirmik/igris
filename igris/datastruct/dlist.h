#ifndef IGRIS_DLIST_HEAD_H
#define IGRIS_DLIST_HEAD_H

//#include <inttypes.h>
#include <sys/cdefs.h>
#include <igris/util/member.h>
#include <igris/dprint.h>

/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define DLIST_POISON1  ((struct dlist_head *) 0xDEADC0DE)
#define DLIST_POISON2  ((struct dlist_head *) 0xDEADC9DE)

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */
struct dlist_head
{
    struct dlist_head *next, *prev;
};

#define DLIST_HEAD_INIT(name) { &(name), &(name) }

#define DLIST_HEAD(name) \
    struct dlist_head name = DLIST_HEAD_INIT(name)

__BEGIN_DECLS

/**
 * Init dlist head.
 *
 * It should be used before all operations with head
 * except dlist_add*,
 */
static inline void dlist_init(struct dlist_head* head)
{
    head->next = head->prev = head;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __dlist_add(struct dlist_head *lnk, struct dlist_head *next, struct dlist_head *prev)
{
    lnk->prev = prev;
    lnk->next = next;
    next->prev = lnk;
    prev->next = lnk;
}

/**
 * Insert a new entry after the specified head.
 * @lnk: new entry to be added
 * @head: list head to add it after
 */
static inline void dlist_add_next(struct dlist_head* lnk, struct dlist_head* head)
{
    __dlist_add(lnk, head->next, head);
}
#define dlist_add(a,b) dlist_add_next(a,b)

/**
 * Insert a new entry before the specified head.
 * @lnk: new entry to be before
 * @head: list head to add it after
 */
static inline void dlist_add_prev(struct dlist_head* lnk, struct dlist_head* head)
{
    __dlist_add(lnk, head, head->prev);
}
#define dlist_add_tail(a,b) dlist_add_prev(a,b)

/**
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __dlist_del(struct dlist_head * prev, struct dlist_head * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * Deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void dlist_del(struct dlist_head *entry)
{
    __dlist_del(entry->prev, entry->next);
    entry->next = DLIST_POISON1;
    entry->prev = DLIST_POISON2;
}

/**
 * Deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void dlist_del_init(struct dlist_head *entry)
{
    __dlist_del(entry->prev, entry->next);
    dlist_init(entry);
}

/**
 * Delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void dlist_move(struct dlist_head *list, struct dlist_head *head)
{
    __dlist_del(list->prev, list->next);
    dlist_add(list, head);
}

/**
 * Delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void dlist_move_tail(struct dlist_head *list, struct dlist_head *head)
{
    __dlist_del(list->prev, list->next);
    dlist_add_tail(list, head);
}
#define dlist_move_prev(a,b) dlist_move_tail(a,b)

/**
 * Tests whether a list is empty
 * @head: the list to test.
 */
static inline int dlist_empty(const struct dlist_head *head)
{
    return head->next == head;
}

__END_DECLS

#define dlist_entry(ptr, type, member) \
    mcast_out(ptr, type, member)

#define dlist_next_entry(pos, member) \
    dlist_entry((pos)->member.next, __typeof__(*(pos)), member)

#define dlist_prev_entry(pos, member) \
    dlist_entry((pos)->member.prev, __typeof__(*(pos)), member)

#define dlist_first_entry(ptr, type, member) \
    dlist_entry((ptr)->next, type, member)

#define dlist_last_entry(ptr, type, member) \
    dlist_entry((ptr)->prev, type, member)

#define dlist_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

#define dlist_for_each_reverse(pos, head) \
for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define dlist_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
    pos = n, n = pos->next)

#define dlist_for_each_entry(pos, head, member)                     \
    for (pos = dlist_first_entry(head, __typeof__(*pos), member);   \
    &pos->member != (head);                                         \
    pos = dlist_next_entry(pos, member))

#define dlist_for_each_entry_safe(pos, n, head, member)             \
    for (pos = dlist_first_entry(head, __typeof__(*pos), member),   \
    n = dlist_next_entry(pos, member);                              \
    &pos->member != (head);                                         \
    pos = n, n = dlist_next_entry(n, member))

__BEGIN_DECLS

static inline
int dlist_in(struct dlist_head *fnd, struct dlist_head *head)
{
    struct dlist_head * it;
    dlist_for_each(it, head)
    {
        if (it == fnd)
            return 1;
    }
    return 0;
}

static inline
int dlist_check(struct dlist_head *fnd, int count)
{
    struct dlist_head * it = fnd;

    while(count--) 
    {
        struct dlist_head * next = it->next;

        if (fnd == next) 
            return 1;

        it = next;         
    }

    return 0;
}

static inline
int dlist_oposite_check(struct dlist_head *fnd, int count)
{
    struct dlist_head * it = fnd;

    while(count--) 
    {
        struct dlist_head * prev = it->prev;

        if (fnd == prev) 
            return 1;

        it = prev;         
    }

    return 0;
}

static inline
void dlist_debug_print_node(const char* prefix, struct dlist_head *node) 
{
    dpr(prefix); dprptr(node); 
    dpr(" (next: "); dprptr(node->next);
    dpr(", prev: "); dprptr(node->prev);
    dprln(")");
} 

static inline
void dlist_debug_print(struct dlist_head *head) 
{
    struct dlist_head * it;

    dlist_debug_print_node("head: ", head); 
    dlist_for_each(it, head) 
    {
        dlist_debug_print_node("node: ", it);
    }
}

static inline 
int dlist_size(struct dlist_head *head) 
{
    struct dlist_head * it;
    int sz = 0;

    dlist_for_each(it, head) 
    {
        ++sz;
    }

    return sz;   
}

static inline 
int dlist_oposite_size(struct dlist_head *head) 
{
    struct dlist_head * it;
    int sz = 0;

    dlist_for_each_reverse(it, head) 
    {
        ++sz;
    }

    return sz;   
}

__END_DECLS

#endif
