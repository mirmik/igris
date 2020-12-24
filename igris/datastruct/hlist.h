#ifndef GENOS_HLIST_HEAD
#define GENOS_HLIST_HEAD

//#include <igris/compiler.h>
#include <igris/compiler.h>

struct hlist_node {
	struct hlist_node* next;
	struct hlist_node** pprev;
};

struct hlist_head {
	struct hlist_node* first;
};

__BEGIN_DECLS

static inline struct hlist_head* hlist_head_init(struct hlist_head* list) {
	list->first = 0; 
	return list;
}

static inline struct hlist_node* hlist_node_init(struct hlist_node* node) {
	node->pprev = 0; 
	return node;
}

static inline void hlist_add_next(struct hlist_node* list, struct hlist_node** hnext) {
	list->pprev = hnext;
	list->next = *hnext;
	if (list->next) list->next->pprev = &(list->next);
	*hnext = list;
}

static inline void hlist_del(struct hlist_node* list) {
	if (list->pprev == 0) return;
	if (list->next) list->next->pprev = list->pprev;	
	*list->pprev = list->next;
}

__END_DECLS

#define hlist_entry(ptr, type, member) \
mcast_out(ptr, type, member)

#define hlist_next_entry(pos, member) \
hlist_entry((pos)->member.next, __typeof__(*(pos)), member)

#define hlist_first_entry(head, type, member) \
hlist_entry((head)->first, type, member)

#define hlist_for_each(pos, head) \
for (pos = (head)->first; pos != 0; pos = pos->next)

#define hlist_for_each_entry(pos, head, member) \
for (pos = hlist_first_entry(head, __typeof__(*pos), member);	\
&pos->member != 0;											\
pos = hlist_next_entry(pos, member))

#endif
