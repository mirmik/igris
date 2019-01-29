#ifndef GXX_DATASTRUCT_GRAPH_H
#define GXX_DATASTRUCT_GRAPH_H

/*#include <gxx/datastruct/dlist_head.h>
#include <sys/cdefs.h>

struct vertex_head {
	struct dlist_head inhead;
	struct dlist_head outhead;
};

struct edge_head {
	struct dlist_head prev;
	struct dlist_head next;
};

__BEGIN_DECLS

static inline void graph_vertex_init(struct vertex_head* vtx) {
	dlist_init(&vtx->inhead);
	dlist_init(&vtx->outhead);
}

static inline void graph_connect(struct edge_head* e, struct vertex_head* v1, struct vertex_head* v2) {
	dlist_add_next(&e->next, &v1->outhead);
	dlist_add_next(&e->prev, &v1->inhead);
}

static inline void graph_disconnect_vertex(struct vertex_head* vtx) {
	struct dlist_head* it;
	dlist_for_each(it, &vtx->inhead) {
		debug_print("AllGood\n");
	}
} 

__END_DECLS*/

#endif